#! /bin/bash
export TOP=$(pwd)
#git clone https://github.com/riscv/riscv-tools.git
#cd $TOP/riscv-tools
#git submodule update --init --recursive
#sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc
#export RISCV=$TOP/riscv
export RISCV=$TOP/riscv
export PATH=$PATH:$RISCV/bin
#sed -i 's/JOBS=16/JOBS=8/' build.common
#./build.sh
#cd $TOP
#echo -e '#include <stdio.h>\n int main(void) { printf("Hello world!\\n"); return 0; }' > hello.c
#riscv64-unknown-elf-gcc -o hello hello.c
#spike pk hello
#echo "execute this"      
#echo "call this, maybe"
#echo "run this in linux"
#export TOP=$(pwd)
#git clone https://github.com/riscv/riscv-tools.git
#cd $TOP/riscv-tools
#git submodule update --init
#sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf

#export PATH=$PATH:$RISCV/bin
#sed -i 's/JOBS=16/JOBS=8/' build.common
#./build-spike-only.sh
#./configure --prefix=$RISCV
#make linux
#cd $TOP
#git clone https://github.com/riscv/riscv-linux.git linux-3.14.33
#curl -L https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.14.33.tar.xz | tar -xJkf -
#make ARCH=riscv defconfig
#make ARCH=riscv menuconfig
#make -j16 ARCH=riscv
#curl -L http://riscv.org/install-guides/busybox-riscv.config > .config
#make menuconfig
#make -j16
#cd $TOP/linux-3.14.33
#dd if=/dev/zero of=root.bin bs=1M count=64
#mkfs.ext2 -F root.bin
#mkdir mnt
#sudo mount -o loop root.bin mnt
#cd mnt
#mkdir -p bin etc dev lib proc sbin sys tmp usr usr/bin usr/lib usr/sbin
#cp $TOP/busybox-1.21.1/busybox bin
#curl -L http://riscv.org/install-guides/linux-inittab > etc/inittab
#ln -s ../bin/busybox sbin/init
#cd ..
#sudo umount mnt
#spike +disk=root.bin bbl vmlinux
#./build.sh
#cd $TOP/linux-3.14.33
#make ARCH=riscv headers_check
#make ARCH=riscv headers_install INSTALL_HDR_PATH=$RISCV/sysroot64/usr
#mkdir mnt
#fuseext2 -o rw+ root.bin mnt
#fusermount -u mnt
#cp $RISCV/sysroot64/lib/libc.so.6 lib/
#cp $RISCV/sysroot64/lib/ld.so.1 lib/
#make -j16
#cd $TOP/linux-3.14.33/mnt
#cp $TOP/busybox-1.21.1/busybox bin
