obj-m +=jiffies.o
obj-m +=seconds.o

all:
	make -C /lib/modules/$(shell uname -r)/build \ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build \ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=$(PWD) clean
