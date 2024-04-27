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

const unsigned char kbdus[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',		/* 9 */
	'9', '0', '-', '=', '\b',				/* Backspace */
	'\t',							/* Tab */
	'q', 'w', 'e', 'r',					/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
	0,							/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,						/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,							/* Alt */
	' ',							/* Space bar */
	0,							/* Caps lock */
	0,							/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,							/* < ... F10 */
	0,							/* 69 - Num lock*/
	0,							/* Scroll Lock */
	0,							/* Home key */
	0,							/* Up Arrow */
	0,							/* Page Up */
	'-',
	0,							/* Left Arrow */
	0,
	0,							/* Right Arrow */
	'+',
	0,							/* 79 - End key*/
	0,							/* Down Arrow */
	0,							/* Page Down */
	0,							/* Insert Key */
	0,							/* Delete Key */
	0,   0,   0,
	0,							/* F11 Key */
	0,							/* F12 Key */
	0,							/* All other keys are undefined */
};

static int check_for_modifiers(unsigned char scancode)
{
	switch (scancode) {
		// presses
		case 0x2A:
			gmodifiers |= LShift;
			goto RETURN_SUCCESS;
			break;
		case 0x1D:
			gmodifiers |= LCtrl;
			goto RETURN_SUCCESS;
			break;
		case 0x38:
			gmodifiers |= LAlt;
			goto RETURN_SUCCESS;
			break;
		case 0x36:
			gmodifiers |= RShift;
			goto RETURN_SUCCESS;
			break;
		// releases
		case 0xAA:
			gmodifiers &= ~LShift;
			goto RETURN_SUCCESS;
		case 0x9D:
			gmodifiers &= ~LCtrl;
			goto RETURN_SUCCESS;
		case 0xB8:
			gmodifiers &= ~LAlt;
			goto RETURN_SUCCESS;
		case 0xB6:
			gmodifiers &= ~RShift;
			goto RETURN_SUCCESS;
		default:
			return 1;
	}

RETURN_SUCCESS:
	return 0;
}

static irqreturn_t irq_handler(int irq, void* dev_id)
{
	static unsigned char 	scancode;
	char			c;

	scancode = inb(0x60);
	if (!check_for_modifiers(scancode))
		goto FINISH;
	c = kbdus[scancode];
	if (c >= 0x20 && c <= 0x7e) {
		printk(KERN_INFO "ps2: key has been pressed: %c\n", c);
	}

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
