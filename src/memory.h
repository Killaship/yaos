/*extern void heap_start;
extern void heap_end;
void *g_heap = (void *)0;
 
void *sbrk(int nbytes)
{
    void *alloc;
    if(!g_heap) g_heap = &heap_start;
    alloc   = g_heap;
    g_heap += nbytes;
    if(g_heap >= &heap_end)
        return (void *)-1; // No more heap
    return alloc;
}
*/
void loadpagedir(uint32_t ptr) {
	asm("push %ebp; mov %esp, %ebp;"); 
	asm("movl %0, %%cr3" ::"r"(ptr));
	
}
void enablepaging() {
	asm volatile("push %ebp; mov %esp, %ebp; mov %cr0, %eax; or $0x80000000, %eax; mov %eax, %cr0; mov %ebp, %esp; pop %ebp;");
}
uint32_t page_directory[1024] __attribute__((aligned(4096)));

//set each entry to not present
void paging_init() {
	int i;
	for(i = 0; i < 1024; i++)
	{
		// write/read, supervisor, not present
		page_directory[i] = 0x00000002;
	}
	// attributes: supervisor level, read/write, present
	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	uint32_t first_page_table[1024] __attribute__((aligned(4096)));
	unsigned int a;
	 
	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(a = 0; a < 1024; a++)
	{
    		// As the address is page aligned, it will always leave 12 bits zeroed.
   		// Those bits are used by the attributes ;)
    		first_page_table[a] = (a * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	page_directory[0] = ((unsigned int)first_page_table) | 3;
	loadpagedir(page_directory);
	enablepaging();
}
