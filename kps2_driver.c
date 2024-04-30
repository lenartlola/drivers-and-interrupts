#include "kps_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lenart");
MODULE_DESCRIPTION("A simple kps2 module");
MODULE_VERSION("0.01");

static int irq = 1;
static unsigned int cookie = 0xdeadd00d;

Modifiers gmodifiers = None;

struct kps_data kps_data = {
	.lock = __MUTEX_INITIALIZER(kps_data.lock),
	.entries = LIST_HEAD_INIT(kps_data.entries),
	.aLen = 0,
};

static void check_for_modifiers(unsigned char scancode)
{
	switch (scancode) {
		// presses
		case 0x2A:
			gmodifiers |= LShift;
			break;
		case 0x3A:
			gmodifiers |= Capslock;
			break;
		case 0x1D:
			gmodifiers |= LCtrl;
			break;
		case 0x38:
			gmodifiers |= LAlt;
			break;
		case 0x36:
			gmodifiers |= RShift;
			break;
		// releases
		case 0xAA:
			gmodifiers &= ~LShift;
			break;
		case 0xBA:
			gmodifiers &= ~Capslock;
			break;
		case 0x9D:
			gmodifiers &= ~LCtrl;
			break;
		case 0xB8:
			gmodifiers &= ~LAlt;
			break;
		case 0xB6:
			gmodifiers &= ~RShift;
			break;
		default:
			break;
	}
}

static irqreturn_t irq_handler(int ir, void* dev_id)
{
	static unsigned char 	scancode;

	scancode = inb(0x60);
	check_for_modifiers(scancode);
	if ((scancode >= 0x01 && scancode <= 0x5d)
		|| (scancode >= 0x81 && scancode <= 0xDD))
	{
		if (new_node(scancode))
			goto FREE_FATAL;
	}

	return IRQ_HANDLED;
FREE_FATAL:
	synchronize_irq(irq);
	free_irq(irq, &cookie);
	deregister_misc_device();
	return -1;
}

static int __init keyboard_ps2(void)
{
	int		ret;

	printk(KERN_INFO "%s: init...\n", __func__);
	ret = register_misc_device();
	if (ret)
		return ret;
	ret = request_irq(irq, irq_handler, IRQF_SHARED, "Keyboard key", &cookie);
	if (ret) {
		pr_err("%s: failed to request irq %d\n", __func__, ret);
		deregister_misc_device();
		return ret;
	}
	return ret;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "%s: cleaning up...\n", __func__);
	synchronize_irq(irq);
	free_irq(irq, &cookie);
	deregister_misc_device();
}

module_init(keyboard_ps2);
module_exit(mod_exit);
