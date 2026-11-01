#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FuShengyuan");
MODULE_DESCRIPTION("Deferred work and kernel thread example");

struct work_ctx
{
    struct work_struct work;
    int current_id;
};

#define WORK_COUNT 10
#define STUDENT_ID 730

static struct delayed_work my_work;
static struct work_ctx works[WORK_COUNT];
static struct task_struct *threads[WORK_COUNT];
static struct workqueue_struct *workqueue;
static DECLARE_COMPLETION(start_barrier);

static int kthread_handler(void *data)
{
    struct work_ctx *ctx = data;

    wait_for_completion(&start_barrier);
    if (!kthread_should_stop())
        pr_info("kthread: %d\n", ctx->current_id);

    return 0;
}

static void work_queue_handler(struct work_struct *work)
{
    struct work_ctx *ctx = container_of(work, struct work_ctx, work);

    pr_info("work queue: %d\n", ctx->current_id);
}

static void delayed_work_handler(struct work_struct *work)
{
    pr_info("deferred work: delayed work\n");
}

static int __init deferred_work_init(void)
{
    int i;
    int ret;

    pr_info("deferred work module init\n");
    INIT_DELAYED_WORK(&my_work, delayed_work_handler);
    workqueue = alloc_ordered_workqueue("deferred_workqueue", 0);
    if (!workqueue)
        return -ENOMEM;

    for (i = 0; i < WORK_COUNT; ++i) {
        works[i].current_id = STUDENT_ID + i;
        INIT_WORK(&works[i].work, work_queue_handler);
        queue_work(workqueue, &works[i].work);
        threads[i] = kthread_run(kthread_handler, &works[i],
                                 "deferred_worker/%d", i);
        if (IS_ERR(threads[i])) {
            int j;

            pr_err("failed to create worker %d\n", i);
            ret = PTR_ERR(threads[i]);
            threads[i] = NULL;
            complete_all(&start_barrier);
            for (j = 0; j < i; ++j)
                kthread_stop(threads[j]);
            destroy_workqueue(workqueue);
            workqueue = NULL;
            return ret;
        }
    }

    complete_all(&start_barrier);
    schedule_delayed_work(&my_work, msecs_to_jiffies(5000));
    return 0;
}

static void __exit deferred_work_exit(void)
{
    int i;

    cancel_delayed_work_sync(&my_work);
    for (i = 0; i < WORK_COUNT; ++i) {
        if (threads[i]) {
            kthread_stop(threads[i]);
            threads[i] = NULL;
        }
    }
    if (workqueue) {
        destroy_workqueue(workqueue);
        workqueue = NULL;
    }

    pr_info("deferred work module exit\n");
}

module_init(deferred_work_init);
module_exit(deferred_work_exit);
