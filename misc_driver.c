#include "kps_driver.h"

static ssize_t etx_misc_write(struct file *file, const char __user *buffer,
							  size_t count, loff_t *ppos)
{
	(void)file;
	(void)buffer;
	(void)count;
	(void)ppos;
	printk(KERN_DEBUG "kps2: attempt to write\n");

	return 0;
}

static ssize_t etx_misc_read(struct file *file, char __user *buffer,
							 size_t len, loff_t *offset)
{
	struct key_entry	*entry;
	int					c;

	while (!list_empty(&kps_data.entries))
	{
		entry = list_first_entry(&kps_data.entries, struct key_entry, list);
		c = entry->k_data.ascii_key == 0x0 ? ' ' : entry->k_data.ascii_key;
		// TODO write to user buffer
//		snprintf(msg, sizeof(msg), "%s(%c)\n", entry->k_data.name, c);
		printk(KERN_INFO "%s(%c)\n",
			   entry->k_data.name, c);
		list_del(&entry->list);
		kfree(entry);
	}
	printk(KERN_DEBUG "kps2: attempt to read\n");
	return 0;
}

struct file_operations fops = {
	.write = etx_misc_write,
	.read = etx_misc_read,
//	.open = etx_misc_open,
//	.release = etc_misc_release,
};

struct miscdevice etx_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "kps_driver",
	.fops = &fops,
};

int register_misc_device(void)
{
	int ret;

	printk(KERN_INFO "%s: register device\n", __func__);
	ret = misc_register(&etx_misc_device);
	printk(KERN_DEBUG "%s: registered device %d\n", __func__, ret);

	return ret;
}

void deregister_misc_device(void)
{
	struct key_entry *entry;

	printk(KERN_INFO "%s: deregister device\n", __func__);
	misc_deregister(&etx_misc_device);

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

