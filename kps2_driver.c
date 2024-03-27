#include <linux/kernel.h>
#include <linux/interrupt.h>
#include<linux/module.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lenart");
MODULE_DESCRIPTION("A simple kps2 module");
MODULE_VERSION("0.01");

static int irq = 1;
static int device = 0xdeadbeef;

static irqreturn_t irq_handler(int irq, void* dev_id)
{
	static unsigned char scancode;

	scancode = inb(0x60);
	printk(KERN_INFO "custom irq handler: pressed key %d\n", scancode);
	return IRQ_NONE;
}

static int __init keyboard_ps2(void)
{
	int	ret;

	printk(KERN_INFO "kps2: init...\n");
	ret = request_irq(irq, irq_handler, IRQF_SHARED, "Keyboard key", &device);
	printk(KERN_INFO "kps2: irq(%d) has been trigerred, res: %d\n", irq, ret);
	return ret;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "kps2: cleaning up...\n");
	synchronize_irq(irq);
	free_irq(irq, &device);
}

module_init(keyboard_ps2);
module_exit(mod_exit);
