#include <linux/kernel.h>
#include <linux/interrupt.h>
#include<linux/module.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lenart");
MODULE_DESCRIPTION("A simple kps2 module");
MODULE_VERSION("0.01");

static int irq = 1;
static int cookie = 0xdeadbeef;

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

Modifiers gmodifiers = None;

static irqreturn_t irq_handler(int irq, void* dev_id)
{
	static unsigned char scancode;

	scancode = inb(0x60);

	if (gmodifiers & RShift) {
		printk(KERN_INFO "kps2: Detected RShift pressed\n");
		goto FINISH;
	}

	if (scancode == 0x36) {
		gmodifiers |= RShift;
		printk(KERN_INFO "kps2: RShift has been pressed\n");
		goto FINISH;
	}

	printk(KERN_INFO "Pressed a normal key %d\n", scancode);
FINISH:
	return IRQ_NONE;
}

static int __init keyboard_ps2(void)
{
	int	ret;

	printk(KERN_INFO "kps2: init...\n");
	ret = request_irq(irq, irq_handler, IRQF_SHARED, "Keyboard key", &cookie);
	printk(KERN_INFO "kps2: irq(%d) has been trigerred, res: %d\n", irq, ret);
	return ret;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "kps2: cleaning up...\n");
	synchronize_irq(irq);
	free_irq(irq, &cookie);
}

module_init(keyboard_ps2);
module_exit(mod_exit);
