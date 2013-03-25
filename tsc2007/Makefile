obj-m += tsc2007.o
obj-m += tsc_raspi.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/input/touchscreen
	cp tsc2007.ko /lib/modules/$(shell uname -r)/kernel/drivers/input/touchscreen/tsc2007.ko
	cp tsc_raspi.ko /lib/modules/$(shell uname -r)/kernel/drivers/input/touchscreen/tsc_raspi.ko
	sed -i '/blacklist i2c-bcm2708/s/^b/#b/' /etc/modprobe.d/raspi-blacklist.conf
	grep -q "i2c-dev" /etc/modules || echo "i2c-dev" | tee -a /etc/modules
	grep -q "tsc_raspi" /etc/modules || echo "tsc_raspi" | tee -a /etc/modules
	grep -q "tsc2007" /etc/modules || echo "tsc2007" | tee -a /etc/modules
	depmod -a

uninstall:
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/input/touchscreen/tsc2007.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/input/touchscreen/tsc_raspi.ko
	sed -i '/tsc2007/d' /etc/modules
	sed -i '/tsc_raspi/d' /etc/modules
	depmod -a

uninstall-i2c:
	sed '/#blacklist i2c-bcm2708/s/#b/b/' /etc/modprobe.d/raspi-blacklist.conf
	sed -i '/i2c-dev/d' /etc/modules
