#include "I2Croutines.h"
#include "stm32f10x_i2c.h"
#include "lighting.h"
#include "queue.h"


#define SLAVE_ADDRESS 0x37<<1
#define ClockSpeed 400000

__IO uint8_t IndexRX, IndexTX;        // this line contain index for store received CMD thru interrupts, 
__IO u8 Direction;                    // and direction data transfer (in Receive eq 1)          
u8 I2C2_Buffer_Rx[8]={0};             // received command buffer for planning transfer
u8 I2C2_Buffer_Tx[4]={0};             // data for transfer (or portion)//exclusive queue
u32 * pQueueOut = (u32 *)I2C2_Buffer_Tx; // we will put from this pointer queue data for transfer;

u8 Status = 0x00;                     //Status Variable 
u8 Command=0xFF;                           // command what We do
u16 WordsTranslate =0x00;                 // what number of words (32 bit) we will translate
 

void ConfigI2C(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  //-- I2C -----------------------------------------------------------------------------------------------
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  
   /* Configure I2C2 pins: SCL and SDA ----------------------------------------*/
  I2C_Cmd(I2C2, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  I2C_DeInit(I2C2);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = SLAVE_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
  I2C_Init(I2C2, &I2C_InitStructure);
 
  I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
  I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
    
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //set highest priority
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
    /* Configure and enable I2C2 error interrupt -------------------------------*/  
   NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_Init(&NVIC_InitStructure); 
}


void I2C2_EV_IRQHandler(void)
{
  static u8 Count=0;
 
  switch (I2C_GetLastEvent(I2C2))
  {
    /* Slave Transmitter ---------------------------------------------------*/
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  /* EV1 */
      /* Transmit I2C2 data */
      Direction = 0; // Go to TX
      ProcessCommand(I2C2_Buffer_Rx);
      I2C2->DR = I2C2_Buffer_Tx[0];
      IndexTX = 1; // reset IndexRX;
      break;


   case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:             /* EV3 */
      /* Transmit I2C2 data */
       Count++;
      I2C2->DR = I2C2_Buffer_Tx[IndexTX++];
      IndexTX &= 0x03;
      if (Command == 0x01)
         if(!IndexTX)
            if(WordsTranslate--)*pQueueOut = queue_get_elem();  // next event from queue
      break; 
  


    /* Slave Receiver ------------------------------------------------------*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:     /* EV1 */
      Direction = 1; // We have got message :)
      IndexRX = 0; // reset IndexRX;
      break;


    case I2C_EVENT_SLAVE_BYTE_RECEIVED:                /* EV2 */
      /* Store I2C2 received data */
      I2C2_Buffer_Rx[IndexRX&0x03] = I2C_ReceiveData(I2C2);           //overflow protect included 
      IndexRX++;
       break; 


    case I2C_EVENT_SLAVE_STOP_DETECTED:                /* EV4 */
      /* Clear I2C2 STOPF flag: read of I2C_SR1 followed by a write on I2C_CR1 */
      (void)(I2C_GetITStatus(I2C2, I2C_IT_STOPF));
      I2C_Cmd(I2C2, ENABLE);
      
      if(Direction == 0x01)
          if(*I2C2_Buffer_Rx == 0x02)    
            LEDs_On(I2C2_Buffer_Rx[1]); // Simply Drive LED
  }
}


/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C2_ER_IRQHandler(void)
{
  /* Check on I2C2 AF flag and clear it */
  if (I2C_GetITStatus(I2C2, I2C_IT_AF)) 
  {
    I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
//         I2C_Cmd(I2C2, DISABLE);
 //         I2C_Cmd(I2C2, ENABLE);
 }
}


void ProcessCommand(u8* pCommand){
 Command = *pCommand;  

  switch(Command){
    case 0x00:
      I2C2_Buffer_Tx[0] = Status;
      I2C2_Buffer_Tx[1] = queue.counter & 0x00FF;
      I2C2_Buffer_Tx[2] = (queue.counter >> 8);
      
      break;
  
    case 0x01:
       WordsTranslate = *((u16*)(pCommand+1));  // number of words we will translate
       *pQueueOut = queue_get_elem(); 
       WordsTranslate --; // the first world is set
 }
return;
}
