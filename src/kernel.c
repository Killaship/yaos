/*
* Copyright (C) 2014  Arjun Sreedharan
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
#define OS_BUILD "0.1" // Update this every release
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "memory.h"

#include "common.h"
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08


void load_idt(unsigned long *idt_ptr) {
	asm volatile ("lidt (%%eax)" :: "a" (idt_ptr));
}

char read_port(unsigned short port) {
	unsigned char value;
	asm volatile ("inb %%dx, %%al" : "=a" (value) : "d" (port));
	return value;
}

void write_port(unsigned short port, unsigned char data) {
	asm volatile ("outb %%al, %%dx" :: "a" (data), "d" (port));
}

/* current cursor location */
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];




void kprint(const char *str, int color)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = color;
	}
}

void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}

#include "panic.h"

void idt_init(void)
{
	
 	unsigned long addr_address[32] = {0};
	unsigned long idt_address;
	unsigned long idt_ptr[2];
	
	int i = 0;
	while(i < 31) {
			addr_address[i] = (unsigned long)idtfunc[i];
			IDT[i].offset_lowerbits = addr_address[i] & 0xffff;
			IDT[i].selector = KERNEL_CODE_SEGMENT_OFFSET;
			IDT[i].zero = 0;
			IDT[i].type_attr = INTERRUPT_GATE;
			IDT[i].offset_higherbits = (addr_address[i] & 0xffff0000) >> 16;	
		i++;
	}
	

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}


void kmain(void)
{
	const char *str = "yaos dev version id:";
	clear_screen();
	kprint(str, 0x07);
	kprint(OS_BUILD, 0x1F);
	kprint_newline();
	kprint_newline();

	idt_init();
	asm volatile("pop");
		
	while(1);
}
