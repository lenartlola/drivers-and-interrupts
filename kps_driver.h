#ifndef DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H
#define DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/ktime.h>
#include <linux/rtc.h>

extern struct miscdevice ct_misc_device;

typedef enum {
	None = 0,
	RShift = 1 << 0,
	LShift = 1 << 1,
	Capslock = 1 << 2,
	RCtrl = 1 << 3,
	LCtrl = 1 << 4,
	RAlt = 1 << 5,
	LAlt = 1 << 6,
} Modifiers;

extern Modifiers gmodifiers;

struct	kps_data {
	struct mutex		lock;
	struct list_head	entries;
	unsigned int		aLen;
};

typedef struct key_data {
	char			name[16];
	int			ascii_key;
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
	int			hh;
	int			mm;
	int			ss;
	struct list_head	list;
};

extern struct kps_data kps_data;
extern t_key_data en_us[0x5E];

int register_misc_device(void);
void deregister_misc_device(void);
int new_node(unsigned char kc);

#endif //DRIVERS_AND_INTERRUPTS_KPS_DRIVER_H
