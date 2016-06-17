export TOP=$(pwd)
export RISCV=$TOP/riscv
export PATH=$PATH:$RISCV/bin
cd linux-3.14.33
spike +disk=root.bin bbl vmlinux

