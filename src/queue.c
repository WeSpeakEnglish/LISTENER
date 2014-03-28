#include "queue.h"


struct Queue queue;
volatile u8 CriticalSecCntr = 0;

void queue_init(void){
queue.head = 0;
queue.tail = 0;
queue.counter = 0;
}


s8 queue_put_elem(u32* Element){
static int new_tail = 0;
EntrCritSection();
	new_tail = ( queue.tail + 1 ) & MASK_OF_QUEUE;
 	
if(queue.head == new_tail){ExtCritSection(); return 0x00;}
	else{ 
	queue.Elements[queue.tail] = *Element;
	queue.tail = new_tail;
        queue.counter++;
        ExtCritSection();
	return 0;
	}
         
}

u32 queue_get_elem(void){
static int returnValue = 0;
EntrCritSection();
if(queue.head == queue.tail){ ExtCritSection();   return 0x00;}
if(queue.counter == 0){ ExtCritSection(); return 0x00;}
  returnValue = queue.Elements[queue.head];
  queue.head = ( queue.head + 1 ) & MASK_OF_QUEUE; 
  queue.counter--;
ExtCritSection();  
return returnValue;
}


inline void EntrCritSection(void)
{
  if(CriticalSecCntr == 0)
  {
    asm("CPSID i");
  }
  // avoid lost of one count in case of simultaneously calling from both places
  ++CriticalSecCntr;
}

inline void ExtCritSection(void)
{
  if(--CriticalSecCntr == 0)
  {
    asm("CPSIE i");
  }
}





