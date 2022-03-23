#pragma once


struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
void stacktrace(unsigned int maxframes) {
	char* addr[16];
	struct stackframe *stk;
	asm volatile("movl %%ebp,%0" : "=r"(stk) ::);
	kprint("Stack trace:", 0x1F);
	for(unsigned int frame = 0; stk && frame < maxframes; ++frame) {
        	// Unwind to previous stack frame
		prntnum(stk->eip,16,' ',addr);
        	kprint(addr, 0x1F);
		kprint_newline();
        	stk = stk->ebp;
	}
}

/*
void trace(uintptr_t ebp) {
    typedef struct Frame {
        struct Frame* ebp;
        uint32_t eip;
    }Frame;

    unsigned int* current = (Frame*)ebp;

	kprint("Stack Trace:", 0x1F);
	kprint_newline();
    	size_t i = 0;
	while(true) {
		if(!current) {
    		break;
		}

        if(current->eip == 0) {
            break;
	}
	
        kprint("EIP:", 0x1F);
	kprint(itoa(i++),0x1F); 
	kprint(itoa(current->eip),0x1F);		
		
        current = current->ebp;
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
	kprint_newline();
	stacktrace(3);
	
	
}


void err_handler() {
	// Say there's been an error, and then hang.
	bsod(0);
	asm volatile ("cli");
	while(1);
}

void err1_handler() {
	// Say there's been an error, and then hang.
	bsod(1);
	asm volatile ("cli");
	while(1);
}

void err2_handler() {
	// Say there's been an error, and then hang.
	bsod(2);
	asm volatile ("cli");
	while(1);
}

void err3_handler() {
	// Say there's been an error, and then hang.
	bsod(3);
	asm volatile ("cli");
	while(1);
}

void err4_handler() {
	// Say there's been an error, and then hang.
	bsod(4);
	asm volatile ("cli");
	while(1);
}

void err5_handler() {
	// Say there's been an error, and then hang.
	bsod(5);
	asm volatile ("cli");
	while(1);
}

void err6_handler() {
	// Say there's been an error, and then hang.
	bsod(6);
	asm volatile ("cli");
	while(1);
}

void err7_handler() {
	// Say there's been an error, and then hang.
	bsod(7);
	asm volatile ("cli");
	while(1);
}

void err8_handler() {
	// Say there's been an error, and then hang.
	bsod(8);
	asm volatile ("cli");
	while(1);
}

void err9_handler() {
	// Say there's been an error, and then hang.
	bsod(9);
	asm volatile ("cli");
	while(1);
}

void err10_handler() {
	// Say there's been an error, and then hang.
	bsod(10);
	asm volatile ("cli");
	while(1);
}

void err11_handler() {
	// Say there's been an error, and then hang.
	bsod(11);
	asm volatile ("cli");
	while(1);
}

void err12_handler() {
	// Say there's been an error, and then hang.
	bsod(12);
	asm volatile ("cli");
	while(1);
}

void err13_handler() {
	// Say there's been an error, and then hang.
	bsod(13);
	asm volatile ("cli");
	while(1);
}

void err14_handler() {
	// Say there's been an error, and then hang.
	bsod(14);
	asm volatile ("cli");
	while(1);
}

void err15_handler() {
	// Say there's been an error, and then hang.
	bsod(15);
	asm volatile ("cli");
	while(1);
}

void err16_handler() {
	// Say there's been an error, and then hang.
	bsod(16);
	asm volatile ("cli");
	while(1);
}

void err17_handler() {
	// Say there's been an error, and then hang.
	bsod(17);
	asm volatile ("cli");
	while(1);
}

void err18_handler() {
	// Say there's been an error, and then hang.
	bsod(18);
	asm volatile ("cli");
	while(1);
}

void err19_handler() {
	// Say there's been an error, and then hang.
	bsod(19);
	asm volatile ("cli");
	while(1);
}

void err20_handler() {
	// Say there's been an error, and then hang.
	bsod(20);
	asm volatile ("cli");
	while(1);
}

void err21_handler() {
	// Say there's been an error, and then hang.
	bsod(21);
	asm volatile ("cli");
	while(1);
}
void err22_handler() {
	// Say there's been an error, and then hang.
	bsod(22);
	asm volatile ("cli");
	while(1);
}
void err23_handler() {
	// Say there's been an error, and then hang.
	bsod(23);
	asm volatile ("cli");
	while(1);
}
void err24_handler() {
	// Say there's been an error, and then hang.
	bsod(24);
	asm volatile ("cli");
	while(1);
}

typedef void (*f)(void);                 //declare typdef

f idtfunc[32] = {&err_handler, &err1_handler, &err2_handler, &err3_handler, &err4_handler, &err5_handler, &err6_handler, &err7_handler,&err8_handler,&err9_handler,&err10_handler,&err11_handler,&err12_handler,&err13_handler,&err14_handler,&err15_handler,&err16_handler,&err17_handler,&err18_handler,&err19_handler,&err20_handler,&err21_handler,&err15_handler,&err15_handler,&err15_handler,&err15_handler,&err22_handler,&err23_handler,&err24_handler,&err15_handler,};      
//make array func of type f,
//the pointer to a function
