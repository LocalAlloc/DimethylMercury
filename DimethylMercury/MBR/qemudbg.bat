@echo off
nasm MBR.asm -o MBR.bin
qemu-system-x86_64 -drive file=MBR.bin,format=raw -monitor stdio -m 256 -no-shutdown -no-reboot -L "C:\Program Files\qemu"