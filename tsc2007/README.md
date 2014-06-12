Raspberry Pi TSC2007
====================

Kernel Modules
--------------
Make sure your system is up to date before continuing
(`sudo apt-get upgrade && sudo apt-get update`).

Before you can build the modules, you need to install the kernel source. One
easy way to do this is by following the directions
[here](https://github.com/notro/rpi-source/wiki) (you don't need menuconfig).

Once you do that, you can build and install the kernel modules.
```
# Build the modules.
cd tsc2007
make

# Switch to root access.
sudo -s

# Install the modules.
mkdir -p /lib/modules/$(uname -r)/kernel/drivers/input/touchscreen
cp tsc2007.ko tsc_raspi.ko /lib/modules/$(uname -r)/kernel/drivers/input/touchscreen/

# Enable the i2c module. Comment out the line "blacklist i2c-bcm2708".
vi /etc/modprobe.d/raspi-blacklist.conf

# If you want the modules loaded on boot, add "tsc_raspi" and "tsc2007" on
# their own lines.
vi /etc/modules

# Rebuild module dependencies.
depmod -a

# You can load the modules with modprobe now or just reboot.
modprobe tsc_raspi
modprobe tsc2007

exit
```

Calibration
-----------
X.Org has an xinput touchscreen calibrator. Fetch the latest source tarball
from [here](www.freedesktop.org/wiki/Software/xinput_calibrator) and compile
it.

```
# Install dependencies.
sudo apt-get install libx11-dev libxext-dev libxi-dev x11proto-input-dev

# Fetch the source tarball. 0.7.5 was the latest version as of 6/12/2014, but
# it may not be anymore. Check the website linked above.
wget http://github.com/downloads/tias/xinput_calibrator/xinput_calibrator-0.7.5.tar.gz

# Extract the source and build it.
tar -xzf xinput_calibrator-*.tar.gz
cd xinput_calibrator-*
./configure
make

# Run the calibrator program.
src/xinput_calibrator
```
