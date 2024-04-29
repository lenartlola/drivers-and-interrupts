#include "kps_driver.h"

int new_node(unsigned char kc)
{
	struct key_entry *new;

	new = kmalloc(sizeof(struct key_entry), GFP_KERNEL);
	if (!new)
		return -1;

	INIT_LIST_HEAD(&new->list);

	new->keycode = kc;
	if (kc >= 0x00 && kc <= 0x5D)
	{
		new->k_data = en_us[kc];
		new->state = Pressed;
	}
	else if (kc >= 0x81 && kc <= 0xdd)
	{
		new->k_data = en_us[kc - 0x80];
		new->state = Released;
	}
	else
	{
		new->k_data = en_us[0];
		new->state = Undefined;
	}
	mutex_lock(&kps_data.lock);
	list_add_tail(&new->list, &kps_data.entries);
	mutex_unlock(&kps_data.lock);
	return 0;
}
