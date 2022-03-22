
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
	kprint_newline();
	kprint(stopcode, 0x1F);
	
}


void err_handler() {
	// Say there's been an error, and then hang.
	bsod("test");
	asm volatile ("cli");
	while(1);
}
