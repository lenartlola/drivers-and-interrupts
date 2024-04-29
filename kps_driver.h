#ifndef DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H
#define DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/list.h>

extern struct miscdevice etx_misc_device;

struct	kps_data {
	struct mutex		lock;
	struct list_head	entries;
};

typedef struct key_data {
	char	name[16];
	char	ascii_key[5];
}	t_key_data;

typedef enum States {
	Undefined = 0,
	Pressed,
	Released,
} e_states;

struct key_entry
{
	unsigned char		keycode;
	t_key_data		k_data;
	e_states		state;
	struct list_head	list;
};

extern struct kps_data kps_data;
extern t_key_data en_us[0x5E];

int register_misc_device(void);
void deregister_misc_device(void);
int new_node(unsigned char kc);

#endif //DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H
