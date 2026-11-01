#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("FuShengyuan");

MODULE_DESCRIPTION("A simple kernel module with a list and two threads.");

static struct list_head my_list;
static struct task_struct *thread1, *thread2;

spinlock_t lock;

// 定义链表节点结构
struct pid_node
{
    int pid;
    char comm[16];
    struct list_head list;
};

static void free_list(void)
{
    struct pid_node *node;
    struct pid_node *next;

    spin_lock(&lock);
    list_for_each_entry_safe(node, next, &my_list, list) {
        list_del(&node->list);
        kfree(node);
    }
    spin_unlock(&lock);
}

// thread1 函数体
static int thread1_func(void *data)
{
    int round;
    struct task_struct *task;

    for (round = 0; round < 5 && !kthread_should_stop(); ++round) {
        rcu_read_lock();
        for_each_process(task) {
            struct pid_node *node;

            if (kthread_should_stop())
                break;

            node = kmalloc(sizeof(*node), GFP_ATOMIC);
            if (!node)
                continue;

            node->pid = task->pid;
            strscpy(node->comm, task->comm, sizeof(node->comm));

            spin_lock(&lock);
            list_add_tail(&node->list, &my_list);
            spin_unlock(&lock);
        }
        rcu_read_unlock();

        if (round < 4)
            msleep_interruptible(1000);
    }

    return 0;
}
// thread2 函数体
static int thread2_func(void *data)
{
    struct pid_node *node;

    while (!kthread_should_stop()) {
        node = NULL;

        spin_lock(&lock);
        if (!list_empty(&my_list)) {
            node = list_first_entry(&my_list, struct pid_node, list);
            list_del(&node->list);
        }
        spin_unlock(&lock);

        if (node) {
            printk(KERN_INFO "pid: %d name: %s\n", node->pid, node->comm);
            kfree(node);
        } else {
            msleep_interruptible(100);
        }
    }

    return 0;
}

// 模块初始化函数
int kernel_module_init(void)
{
    printk(KERN_INFO "List and thread module init\n");

    INIT_LIST_HEAD(&my_list);
    spin_lock_init(&lock);

    thread1 = kthread_create(thread1_func, NULL, "pid_scanner");
    if (IS_ERR(thread1)) {
        thread1 = NULL;
        return -ENOMEM;
    }
    wake_up_process(thread1);

    thread2 = kthread_create(thread2_func, NULL, "pid_printer");
    if (IS_ERR(thread2)) {
        thread2 = NULL;
        kthread_stop(thread1);
        thread1 = NULL;
        free_list();
        return -ENOMEM;
    }
    wake_up_process(thread2);

    return 0;
}

// 模块清理函数
void kernel_module_exit(void)
{
    // 停止线程1
    if (thread1) {
        kthread_stop(thread1);
        thread1 = NULL;
    }

    // 停止线程2
    if (thread2) {
        kthread_stop(thread2);
        thread2 = NULL;
    }

    // 清理链表
    free_list();

    printk(KERN_INFO "List and thread module exit\n");
}

module_init(kernel_module_init);

module_exit(kernel_module_exit);