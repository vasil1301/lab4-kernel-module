// SPDX-License-Identifier: GPL-2.0
/*
 * Lab 4: kernel module with a parameter and a linked list
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Student <student@example.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times to print \"Hello, world!\" (default 1, max 10)");

struct hello_event {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static void hello_free_list(bool print)
{
	struct hello_event *ev, *tmp;

	list_for_each_entry_safe(ev, tmp, &hello_list, list) {
		if (print)
			pr_info("event time: %lld ns\n", ktime_to_ns(ev->time));
		list_del(&ev->list);
		kfree(ev);
	}
}

static int __init hello_init(void)
{
	uint i;

	if (count > 10) {
		pr_err("count=%u is too big (must be <= 10)\n", count);
		return -EINVAL;
	}

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("count=%u: unusual value, continuing\n", count);

	for (i = 0; i < count; i++) {
		struct hello_event *ev = kmalloc(sizeof(*ev), GFP_KERNEL);

		if (!ev) {
			hello_free_list(false);
			return -ENOMEM;
		}
		ev->time = ktime_get();
		list_add_tail(&ev->list, &hello_list);
		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	hello_free_list(true);
}

module_init(hello_init);
module_exit(hello_exit);
