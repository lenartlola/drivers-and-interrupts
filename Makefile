# kernel tarbal -> https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.8.tar.xz

obj-m += kps2_driver.o

all:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
