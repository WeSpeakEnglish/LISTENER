#include "queue.h"


struct Queue queue;

void queue_init(void){
queue.head = 0;
queue.tail = 0;
queue.counter = 0;
}


s8 queue_put_elem(u32* Element){
static int new_tail = 0;
	new_tail = ( queue.tail + 1 ) & MASK_OF_QUEUE;
	
   if(queue.head == new_tail) return -1;
	else{ 
	queue.Elements[queue.tail] = *Element;
	queue.tail = new_tail;
        queue.counter++;
	return 0;
	}
}

u32 queue_get_elem(void){
static int returnValue = 0;
  if(queue.head == queue.tail) return -1;
  returnValue = queue.Elements[queue.head];
  queue.head = ( queue.head + 1 ) & MASK_OF_QUEUE; 
  queue.counter--;
return returnValue;
}







