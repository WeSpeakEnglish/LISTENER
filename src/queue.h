#ifndef _queue_h
#define _queue_h

#include "stm32f10x.h"

#define SIZE_OF_QUEUE 1024
#define MASK_OF_QUEUE 0x3FF


struct Queue{
	u32 Elements[SIZE_OF_QUEUE];
	int head;
	int tail;
        u16 counter;
}; 

struct QueueElement{
        u8 EventType;
        u8 Keyb_Key;
        u8 RCU_Key;
        u8 Reserved; 
};

union QueueElement_U{
  struct QueueElement QueEl;
  u32 Bits;
};

extern struct Queue queue;

u32 queue_get_elem();
s8 queue_put_elem(u32* Element);
void queue_init(void);

inline void EntrCritSection(void);
inline void ExtCritSection(void);


#endif
