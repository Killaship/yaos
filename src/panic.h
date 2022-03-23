
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
void bsod(const int stopcode) {
	
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
	switch(stopcode) {
		case 0: // #DE
			kprint("Error type: #DE", 0x1F);
			break;
		case 1: // #DB
			kprint("Error type: #DB", 0x1F);
			break;
		case 2: // NMI
			kprint("Error type: NMI", 0x1F);
			break;
		case 3: // #BP
			kprint("Error type: #BP", 0x1F);
			break;
		case 4: // #OF
			kprint("Error type: #OF", 0x1F);
			break;
		case 5: // #BR
			kprint("Error type: #BR", 0x1F);
			break;
		case 6: // #UD
			kprint("Error type: #UD", 0x1F);
			break;
		case 7: // #NM
			kprint("Error type: #NM", 0x1F);
			break;
		case 8: // #DF
			kprint("Error type: #DF", 0x1F);
			break;
		case 9: // CSO
			kprint("Error type: CSO", 0x1F);
			break;
		case 10: // #TS
			kprint("Error type: #TS", 0x1F);
			break;
		case 11: // #NP
			kprint("Error type: #NP", 0x1F);
			break;
		case 12: // #SS
			kprint("Error type: #SS", 0x1F);
			break;
		case 13: // #GP
			kprint("Error type: #GP", 0x1F);
			break;
		case 14: // #PF
			kprint("Error type: #PF", 0x1F);
			break;
		case 15: // Intel Reserved
			kprint("Error type: INTELRSVD", 0x1F);
			break;
		case 16: // #MF
			kprint("Error type: #MF", 0x1F);
			break;
		case 17: // #AC
			kprint("Error type: #AC", 0x1F);
			break;
		case 18: // #MC
			kprint("Error type: #MC", 0x1F);
			break;
		case 19: // #XF
			kprint("Error type: #XF", 0x1F);
			break;
		case 20: // #VE
			kprint("Error type: #VE", 0x1F);
			break;
		case 21: // #CP
			kprint("Error type: #CP", 0x1F);
			break;
		case 22: // #HV
			kprint("Error type: #HV", 0x1F);
			break;
		case 23: // #VC
			kprint("Error type: #VC", 0x1F);
			break;
		case 24: // #SX
			kprint("Error type: #SX", 0x1F);
			break;
	}
	
	
}


void err_handler() {
	// Say there's been an error, and then hang.
	bsod(0);
	asm volatile ("cli");
	while(1);
}
