/*
* Copyright (C) 2014  Arjun Sreedharan
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
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
/*

struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
void stacktrace(unsigned int maxframes)
{
    struct stackframe *stk;
    asm ("movl %%ebp,%0" : "r"(stk) ::);
    kprint("Stack trace:", 0x1F);
    for(unsigned int frame = 0; stk && frame < maxframes; ++frame)
    {
        // Unwind to previous stack frame
        kprint("  0x{0:16}     \n", stk->eip);
        stk = stk->ebp;
    }
}
*/
void bsod(const char* stopcode) {
	
	write_port(0x3D4, 0x0A); // Disable VGA cursor
	write_port(0x3D5, 0x20);
	
	current_loc = 0; // Paint screen blue
  	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x11;
	}
	kprint("A problem has been detected and YAOS has been shut down to prevent damage to" , 0x1F); // text and technical info
  	kprint_newline();
  	kprint("your computer.", 0x1F);
	kprint_newline();
	kprint_newline();
	kprint("If this is the first time you've seen this stop error screen, restart" , 0x1F); 
	kprint_newline();
	kprint("your computer. If this screen appears again, follow these steps:" , 0x1F);
	kprint_newline();
	kprint_newline();
	kprint("Check to make sure any new hardware or software is properly installed.", 0x1F);
	kprint_newline();
	kprint("If this is a new install, check your software/hardware vendor for", 0x1F);
	kprint_newline();
	kprint("any drivers or OS updates you may need.", 0x1F);
	kprint_newline();
	kprint("If problems continue, remove any newly installed hardware or software.", 0x1F);
	kprint_newline();
	kprint("Disable BIOS memory options such as caching or shadowing.", 0x1F);
	kprint_newline();
	kprint_newline();
	kprint("Finally, if all else fails, go to https://github.com/Killaship/yaos/issues", 0x1F);
	kprint_newline();
	kprint("and submit an issue with the below information:", 0x1F);
	kprint_newline();
	
}
void err_handler() {
	// Say there's been an error, and then hang.
	bsod("test");
	asm volatile ("cli");
	while(1);
}

void idt_init(void)
{
	unsigned long err_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];
  int i = 0;
  while(i != 31) {
    err_address = (unsigned long)err_handler;
    IDT[i].offset_lowerbits = err_address & 0xffff;
	  IDT[i].selector = KERNEL_CODE_SEGMENT_OFFSET;
	  IDT[i].zero = 0;
	  IDT[i].type_attr = INTERRUPT_GATE;
	  IDT[i].offset_higherbits = (err_address & 0xffff0000) >> 16;
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
	const char *str = "yaos dev";
	clear_screen();
	kprint(str, 0x07);
	kprint_newline();
	kprint_newline();

	idt_init();
  	int x = 0 / 0;
  	kprint((char*) x, 0x07);

	while(1);
}
