#include "kps_driver.h"

/*
 * seq_file:
 * Create virtual file that can provide information to users.
 * virtual file seq_file interface aspects:
 * 1. An iterator of objects to virtual file to present.
 * 2. Utility functions to format the output.
 * 3. A set of file_operations to operate on the virtual file.
 */

static void *ct_seq_start(struct seq_file *sf, loff_t *pos)
{
	struct key_entry	*p;
	loff_t 				i;

	mutex_lock(&kps_data.lock);
	if (list_empty(&kps_data.entries))
		goto UNLOCK;
	p = list_first_entry(&kps_data.entries, struct key_entry, list);
	for (i = 0; i < *pos && !list_is_last(&p->list, &kps_data.entries); i++)
		p = list_next_entry(p, list);
	if (i != *pos)
		goto UNLOCK;
	mutex_unlock(&kps_data.lock);
	return p;

UNLOCK:
	mutex_unlock(&kps_data.lock);
	return 0;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct key_entry	*p;
	struct key_entry	*next;

	(*pos)++;
	p = (struct key_entry *)v;

	mutex_lock(&kps_data.lock);
	if (list_is_last(&p->list, &kps_data.entries))
		next = NULL;
	else
		next = list_next_entry(p, list);
	mutex_unlock(&kps_data.lock);
	return next;
	printk(KERN_DEBUG "%s: called\n", __func__);
//	return seq_list_next(v, &kps_data.entries, pos);
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
	printk(KERN_DEBUG "%s: called\n", __func__);
}

static int ct_seq_show(struct seq_file *sf, void *v)
{
	struct key_entry *p = (struct key_entry*)v;
	seq_printf(sf, "%s\n", p->k_data.name);
	return 0;
}

static const struct seq_operations ct_seq_ops = {
        .start = ct_seq_start,
		.next = ct_seq_next,
		.stop = ct_seq_stop,
		.show = ct_seq_show,
};

static int ct_open(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "%s: called\n", __func__ );
	file->private_data = NULL;
	return seq_open(file, &ct_seq_ops);
}

struct file_operations fops = {
	.open = ct_open,
	.read = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};

struct miscdevice ct_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "kps_driver",
	.fops = &fops,
};

int register_misc_device(void)
{
	int ret;

	printk(KERN_INFO "%s: register device\n", __func__);
	ret = misc_register(&ct_misc_device);
	printk(KERN_DEBUG "%s: registered device %d\n", __func__, ret);

	return ret;
}

void deregister_misc_device(void)
{
	struct key_entry *entry;

	printk(KERN_INFO "%s: deregister device\n", __func__);
	misc_deregister(&ct_misc_device);

	mutex_lock(&kps_data.lock);
	while (!list_empty(&kps_data.entries)) {
		entry = list_first_entry(&kps_data.entries,
			struct key_entry, list);
		if (strcmp(entry->k_data.name, "None"))
			printk(KERN_INFO "%s: 0x%x - %s\n", __func__, entry->keycode, entry->k_data.name);
		list_del(&entry->list);
		kfree(entry);
	}
	mutex_unlock(&kps_data.lock);

	printk(KERN_INFO "%s: deregistered device\n", __func__);
}

