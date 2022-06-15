#include "stm32f10x.h"
#include "ff.h"
#include "WavePlayer.h"
#include "stm32f10x_dac.h"

#define WP_ENABLE  TIM6->CR1 |=  TIM_CR1_CEN; //��������� ���������������
#define WP_DISABLE TIM6->CR1 &= ~TIM_CR1_CEN; //���������� ���������������

unsigned char DAC_Buff[SIZE_DAC_BUFF];        //����� ������ ����

//********************************************************************************************
//function ��������� ���� � ��������� ��������� ������� ������ format                       //
//argument ��������� �� �������� ���������, ��� �����(� ��������� ���� � ����)              //
//result   WP_OK - ���� ������ ���������, WP_ERROR_FILE - ������ ��������                   //
//********************************************************************************************
WPRESULT wp_open( FIL *file,  const char *FileName, wp_format *format )
{
  unsigned char res;                          //��� �������� ����������
  unsigned char buff[64];                     //����� ��� ��������� �����
  UINT cnt;                          //���������� ����������� ����
  unsigned long i;                            //�����. ����������

  res = f_open ( file, FileName, FA_READ );   //������� ���� ��� ������
  if(res) return WP_ERROR_FILE;               //������ ��������
  res = f_read ( file, &buff[0], 64, &cnt );  //��������� ������ 64 ����
  if(res || cnt!=64) WP_ERROR_FILE;           //������ ������

  //����� 0..3 - ������ ���� ������ "RIFF"
  if(buff[0]!='R' || buff[1]!='I' || buff[2]!='F' || buff[3]!='F')     res = 2;

  //����� 8..11 - ������ ���� ������ "WAVE"
  if(buff[8]!='W' || buff[9]!='A' || buff[10]!='V' || buff[11]!='E')   res = 2;

  //����� 12..15 - ������ ���� ������ "fmt "
  if(buff[12]!='f' || buff[13]!='m' || buff[14]!='t' || buff[15]!=' ') res = 2;

  //����� 20,21  - 1 � 0
  if(buff[20]!=1 || buff[21]!=0) res = 2;

  //��������� ������ ����� "fmt "
  i = buff[16] | (buff[17]<<8) | (buff[18]<<16) | (buff[19]<<24);

  //����� ����� "fmt " ������ ��������� ���� "data"
  //if(buff[20+i]!='d' || buff[21+i]!='a' || buff[22+i]!='t' || buff[23+i]!='a') res = 2;

  if(res==2)
  {
    f_close(file);                          //������� ����
    return WP_ERROR_FILE;                   //������ ������� �����
  }

  /*                    ��������� ������ ������                        */
  format->Channels       = buff[22];
  format->SamplesPerSec  = buff[24] | (buff[25]<<8) | (buff[26]<<16) | (buff[27]<<24);
  format->AvgBytesPerSec = buff[28] | (buff[29]<<8) | (buff[30]<<16) | (buff[31]<<24);
  format->BitsPerSample  = buff[34] | (buff[35]<<8);
  format->Size           = buff[24+i]|(buff[25+i]<<8)|(buff[26+i]<<16)|(buff[27+i]<<24);

  //�������� ��������� �� ������ ������
  f_lseek(file,i+28);

  return WP_OK;
}

//********************************************************************************************
//function ����������� ��������������� ��� ��������� ������ ������                          //
//argument ��������� �� ���������, ���������� ���������� � ������� ������                   //
//********************************************************************************************
void wp_init (wp_format *format)
{
  static char StartInit = 0;                     //������� ��������� �������������

  //��� ������ ��������� � ������� ��������� ��������� �������������
  if(StartInit==0)
  {
    //����������� ������ ����� ���� ��� ������ ������ �� ���
    RCC->APB2ENR        |= RCC_APB2ENR_IOPAEN;     //������������ ����� GPIOA
 //   RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // DAC ON
    DAC->CR             |= DAC_CR_DMAEN1;          //��������� ����� ������ ������ �1 �� ���
    DAC->CR             |= DAC_CR_EN1;             //�������� ����� 1
    //��������� 3-� ����� ��� ��� ������ � �����
    RCC->AHBENR         |= RCC_AHBENR_DMA1EN;      //������������ DMA
    DMA1_Channel3->CMAR  = (uint32_t)&DAC_Buff[0]; //��������� �� ������ ������ � ������
    DMA1_Channel3->CCR   =  DMA_CCR3_DIR;          //�������� �� ������ � ���������
    DMA1_Channel3->CCR  &= ~DMA_CCR3_PINC;         //����� ��������� �� ��������������
    DMA1_Channel3->CCR  |=  DMA_CCR3_MINC;         //����� ������ ��������������
    DMA1_Channel3->CCR  |=  DMA_CCR3_CIRC;         //�������� ����������� ����� �������� ������
    //������ �������� ���������� �������� ����� ��� ����� ����������� �� TIM6
 //   RCC->APB1ENR        |=  RCC_APB1ENR_TIM6EN;    //������ ������������ TIM6
    TIM6->PSC            =  0;                     //������������ �� ���������� (���� =1)
    TIM6->DIER          |=  TIM_DIER_UDE;          //��������� ������ DMA ��� ����������
    //��������� ������������� ���������, ������������ ������� �����
    StartInit            = 1;
  }

  DMA1_Channel3->CCR  &= ~DMA_CCR3_EN;             //��������� ������ ����� DMA

  if(format->BitsPerSample==8)
  {
    DMA1_Channel3->CPAR  = (uint32_t)&DAC->DHR8R1; //��������� �� �������-��������
    DMA1_Channel3->CCR  &= ~DMA_CCR3_PSIZE;        //����������� ������ ��������� - 8 ���.
    DMA1_Channel3->CCR  &= ~DMA_CCR3_MSIZE;        //����������� ������ ������    - 8 ���
    DMA1_Channel3->CNDTR = SIZE_DAC_BUFF;          //������ ������
  }

  if(format->BitsPerSample==16)
  {
    DMA1_Channel3->CPAR  = (uint32_t)&DAC->DHR12L1; //��������� �� �������-��������
    DMA1_Channel3->CCR  |=  DMA_CCR3_PSIZE_0;      //����������� ������ ��������� - 8 ���.
    DMA1_Channel3->CCR  |=  DMA_CCR3_MSIZE_0;      //����������� ������ ������    - 8 ���
    DMA1_Channel3->CNDTR = SIZE_DAC_BUFF;///2;          //������ ������
  }

  DMA1_Channel3->CCR  |=  DMA_CCR3_EN;           //��������� ������ �������� ������ DMA

  //������ ������� �������������
  TIM6->ARR = (uint32_t)FREQ_CLK / format->SamplesPerSec;
}

void wp_convert_data(char numb)
{int i,tmp;
unsigned int data;

 if(numb==0)
 for(i=0;i<256;i+=2)
 {
  data = (unsigned int)(DAC_Buff[i]<<8) | DAC_Buff[i+1];
  data = data + 0x7fff;
  DAC_Buff[i] = data>>8;
  DAC_Buff[i+1] = data;
 }
 if(numb==1)
 for(i=256;i<512;i+=2)
 {
  data = (unsigned int)(DAC_Buff[i]<<8) | DAC_Buff[i+1];
  data = data + 0x7fff;
  DAC_Buff[i] = data>>8;
  DAC_Buff[i+1] = data;
 }
}
//********************************************************************************************
//function ������������� ��������� ����                                                     //
//argument ��� �����(� ��������� ���� � ����)                                               //
//result   ������ �������� WPRESULT � ������������ �����                                    //
//********************************************************************************************
WPRESULT wp_play ( const char *FileName )
{
  FIL           file;                       //�������� ����������
  wp_format     format;                     //������ ���� � ������� �������� ������
  unsigned int cnt;                        //��� �������� ���������� ����������� ����
  WPRESULT      res;

  res = wp_open(&file, FileName, &format);  //������� ���� ��� ���������������
  if(res) return WP_ERROR_FILE;             //������ �������� �����

  wp_init(&format);                         //��������� ��������������� �������� �������

  /*     ��������� ���� ����� ���� ������� �� ����� � ��������� ������������     */
  if( f_read (&file, &DAC_Buff[0], SIZE_DAC_BUFF, &cnt) )
  {
    f_close(&file);                         //������ ��� ������!!! ��������� ����
    return WP_ERROR_READ_FILE;              //� �������
  }

  WP_ENABLE                                 //��������� ��������������

  /*                  ��������������� �����                  */
  while(1)
  {
    while(!(DMA1->ISR & DMA_ISR_HTIF3));      //���� ������������ ������ ����� ������
    DMA1->IFCR |= DMA_ISR_HTIF3;              //�������� ����
    //��������� �������������� ����� ������
    res = f_read ( &file, &DAC_Buff[0], SIZE_DAC_BUFF/2, &cnt );
    if(res) {res = WP_ERROR_READ_FILE; break;}//������ ��� ������!!!
    if(cnt<SIZE_DAC_BUFF/2) break;            //���� ����������

    while(!(DMA1->ISR & DMA_ISR_TCIF3));      //���� ������������ ������ ����� ������
    DMA1->IFCR |= DMA_ISR_TCIF3;              //�������� ����
    //��������� �������������� ����� ������
    res = f_read ( &file, &DAC_Buff[SIZE_DAC_BUFF/2], SIZE_DAC_BUFF/2, &cnt );
    if(res) {res = WP_ERROR_READ_FILE; break;}//������ ��� ������!!!
    if(cnt<SIZE_DAC_BUFF/2) break;            //���� ����������

    //���������� ��������� ���������������
   // res = wp_control();
   // if(res!=WP_NONE)
   // {
   //   if(res==WP_PAUSE)                       //������� �����
   //   {
   //      WP_DISABLE;                          //���������� ��������������
   //      while(res!=WP_PLAY&&res!=WP_STOP)    //������� play ��� stop
   //      res = wp_control();
   //   }
   //   if(res==WP_STOP) break;                 //���� ������� ����, ��������� ����
   //   WP_ENABLE;                              //����������� ��������������
   // }
  }


  WP_DISABLE                                  //���������� ��������������
  f_close(&file);                             //������� ���� ����� �������
  return res;                                 //���������� ���������

}
//********************************************************************************************
//function ��� ���������� ��������� ������                                                  //
//resultt  ��������� ��������                                                               //
//********************************************************************************************
WPRESULT wp_control(void)
{
	/*����� ����� �������� ��� ��� ���������� ����������������.
	  ��������, ��� ������� ������ �1 ������� WP_PAUSE
	            ��� ������� ������ �2 ������� WP_PLAY
	            ��� ������� ������ �3 ������� WP_STOP
	*/
//	if(get_key()==1) return WP_PAUSE;
//	if(get_key()==2) return WP_PLAY;
//	if(get_key()==3) return WP_STOP;
	return WP_NONE;
}
