#include "kps_driver.h"

int new_node(unsigned char kc)
{
	struct key_entry *new;

	printk(KERN_INFO "%s: add 0x%x\n", __func__, kc );

	new = kmalloc(sizeof(struct key_entry), GFP_KERNEL);
	if (!new)
		return -1;

	INIT_LIST_HEAD(&new->list);

	new->keycode = kc;
	if (kc >= 0x00 && kc <= 0x56)
		new->k_data = en_us[kc];
	else
		new->k_data = en_us[0];

	mutex_lock(&kps_data.lock);
	list_add_tail(&new->list, &kps_data.entries);
	mutex_unlock(&kps_data.lock);
	return 0;
}
