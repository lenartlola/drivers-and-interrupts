TARGET= keylogger

obj-m += $(TARGET).o
$(TARGET)-y := kps2_driver.o misc_driver.o list_utils.o en_us.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	insmod keylogger.ko

remove:
	rmmod keylogger
