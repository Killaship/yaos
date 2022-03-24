extern void heap_start;
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
