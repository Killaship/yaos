
compile() {
        cd src
        nasm -f bin program.asm -o program
        nasm -f elf32 kernel.asm -o kasm.o
        cd -
        gcc -Wall -Wextra -fno-stack-protector -m32 -g -c src/kernel.c -o kc.o -ffreestanding

        ld -m elf_i386 -T link.ld -g -o kernel.elf src/kasm.o kc.o
        mkdir -p iso/boot/grub
        mkdir -p iso/modules
        cp src/program iso/modules
        cp kernel.elf iso/boot/kernel.elf
        cp grub.cfg iso/boot/grub/grub.cfg
        grub-mkrescue -o os.iso iso
}

# Get the options
while getopts ":nrc" option; do
   case $option in
      h) # help message
         echo "Usage:"
         echo "-r            || Compile the YAOS source code, and then runs it in QEMU."
         echo "-n            || Just runs YAOS in QEMU. Requires os.iso to be present from prior compilings."
         echo "-c            || Just compiles the YAOS source code, without running it."
         echo "anything else, or -h || Shows this help text."
         exit;;
      r) # compile and run
         compile
         qemu-system-i386 -cdrom os.iso -rtc base=localtime -curses
         exit;;
      c) # just compile
        compile
        exit;;
      n) # just run
         qemu-system-i386 -cdrom os.iso -rtc base=localtime -curses
         exit;;
      \?) # invalid option, shows help message
         echo "Usage:"
         echo "-r            || Compile the YAOS source code, and then runs it in QEMU."
         echo "-n            || Just runs YAOS in QEMU. Requires os.iso to be present from prior compilings."
         echo "-c            || Just compiles the YAOS source code, without running it."
         echo "anything else, or -h || Shows this help text."
         exit;;

      esac
done
