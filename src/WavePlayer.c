#include "stm32f10x.h"
#include "ff.h"
#include "WavePlayer.h"
#include "stm32f10x_dac.h"

#define WP_ENABLE  TIM6->CR1 |=  TIM_CR1_CEN; //включение преобразователя
#define WP_DISABLE TIM6->CR1 &= ~TIM_CR1_CEN; //выключение преобразователя

unsigned char DAC_Buff[SIZE_DAC_BUFF];        //буфер данных ЦАПа

//********************************************************************************************
//function открывает файл и заполняет структуру формата данных format                       //
//argument указатель на файловую структуру, имя файла(с указанием пути к нему)              //
//result   WP_OK - файл открыт нормально, WP_ERROR_FILE - ошибка открытия                   //
//********************************************************************************************
WPRESULT wp_open( FIL *file,  const char *FileName, wp_format *format )
{
  unsigned char res;                          //для хранения результата
  unsigned char buff[64];                     //буфер для заголовка файла
  UINT cnt;                          //количество прочитанных байт
  unsigned long i;                            //вспом. переменная

  res = f_open ( file, FileName, FA_READ );   //открыть файл для чтения
  if(res) return WP_ERROR_FILE;               //ошибка открытия
  res = f_read ( file, &buff[0], 64, &cnt );  //загрузить первые 64 байт
  if(res || cnt!=64) WP_ERROR_FILE;           //ошибка чтения

  //байты 0..3 - должен быть маркер "RIFF"
  if(buff[0]!='R' || buff[1]!='I' || buff[2]!='F' || buff[3]!='F')     res = 2;

  //байты 8..11 - должен быть маркер "WAVE"
  if(buff[8]!='W' || buff[9]!='A' || buff[10]!='V' || buff[11]!='E')   res = 2;

  //байты 12..15 - должен быть маркер "fmt "
  if(buff[12]!='f' || buff[13]!='m' || buff[14]!='t' || buff[15]!=' ') res = 2;

  //байты 20,21  - 1 и 0
  if(buff[20]!=1 || buff[21]!=0) res = 2;

  //вычислить размер чанка "fmt "
  i = buff[16] | (buff[17]<<8) | (buff[18]<<16) | (buff[19]<<24);

  //после чанка "fmt " должен следовать чанк "data"
  //if(buff[20+i]!='d' || buff[21+i]!='a' || buff[22+i]!='t' || buff[23+i]!='a') res = 2;

  if(res==2)
  {
    f_close(file);                          //закрыть файл
    return WP_ERROR_FILE;                   //ошибка формата файла
  }

  /*                    Загрузить формат данных                        */
  format->Channels       = buff[22];
  format->SamplesPerSec  = buff[24] | (buff[25]<<8) | (buff[26]<<16) | (buff[27]<<24);
  format->AvgBytesPerSec = buff[28] | (buff[29]<<8) | (buff[30]<<16) | (buff[31]<<24);
  format->BitsPerSample  = buff[34] | (buff[35]<<8);
  format->Size           = buff[24+i]|(buff[25+i]<<8)|(buff[26+i]<<16)|(buff[27+i]<<24);

  //файловый указатель на начало данных
  f_lseek(file,i+28);

  return WP_OK;
}

//********************************************************************************************
//function настраивает преобразователь под выбранный формат данных                          //
//argument указатель на структуру, содержащей информацию о формате данных                   //
//********************************************************************************************
void wp_init (wp_format *format)
{
  static char StartInit = 0;                     //признак стартовой инициализации

  //При первом обращении к функции выполнить стартовую инициализацию
  if(StartInit==0)
  {
    //Настраиваем первый канал ЦАПа для приема данных от ДМА
    RCC->APB2ENR        |= RCC_APB2ENR_IOPAEN;     //тактирование линий GPIOA
 //   RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // DAC ON
    DAC->CR             |= DAC_CR_DMAEN1;          //разрешить прием данных канала №1 от ДМА
    DAC->CR             |= DAC_CR_EN1;             //включить канал 1
    //Настроить 3-й канал ДМА для работы с ЦАПом
    RCC->AHBENR         |= RCC_AHBENR_DMA1EN;      //тактирование DMA
    DMA1_Channel3->CMAR  = (uint32_t)&DAC_Buff[0]; //указатель на начало буфера в памяти
    DMA1_Channel3->CCR   =  DMA_CCR3_DIR;          //передача из памяти в периферию
    DMA1_Channel3->CCR  &= ~DMA_CCR3_PINC;         //адрес периферии не инкрементируем
    DMA1_Channel3->CCR  |=  DMA_CCR3_MINC;         //адрес памяти инкрементируем
    DMA1_Channel3->CCR  |=  DMA_CCR3_CIRC;         //включить циклический режим передачи данных
    //Запуск передачи очередного значения через ДМА будет происходить от TIM6
 //   RCC->APB1ENR        |=  RCC_APB1ENR_TIM6EN;    //подаем тактирование TIM6
    TIM6->PSC            =  0;                     //предделитель не используем (кофф =1)
    TIM6->DIER          |=  TIM_DIER_UDE;          //разрешить запрос DMA при обновлении
    //Стартовая инициализация выполнена, устанавливам признак этого
    StartInit            = 1;
  }

  DMA1_Channel3->CCR  &= ~DMA_CCR3_EN;             //отключить третий канал DMA

  if(format->BitsPerSample==8)
  {
    DMA1_Channel3->CPAR  = (uint32_t)&DAC->DHR8R1; //указатель на регистр-приемник
    DMA1_Channel3->CCR  &= ~DMA_CCR3_PSIZE;        //размерность данных периферии - 8 бит.
    DMA1_Channel3->CCR  &= ~DMA_CCR3_MSIZE;        //размерность данных памяти    - 8 бит
    DMA1_Channel3->CNDTR = SIZE_DAC_BUFF;          //размер буфера
  }

  if(format->BitsPerSample==16)
  {
    DMA1_Channel3->CPAR  = (uint32_t)&DAC->DHR12L1; //указатель на регистр-приемник
    DMA1_Channel3->CCR  |=  DMA_CCR3_PSIZE_0;      //размерность данных периферии - 8 бит.
    DMA1_Channel3->CCR  |=  DMA_CCR3_MSIZE_0;      //размерность данных памяти    - 8 бит
    DMA1_Channel3->CNDTR = SIZE_DAC_BUFF;///2;          //размер буфера
  }

  DMA1_Channel3->CCR  |=  DMA_CCR3_EN;           //разрешаем работу третьего канала DMA

  //задаем частоту дискретизации
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
//function воспроизводит требуемый файл                                                     //
//argument имя файла(с указанием пути к нему)                                               //
//result   смотри значения WPRESULT в заголовочном файле                                    //
//********************************************************************************************
WPRESULT wp_play ( const char *FileName )
{
  FIL           file;                       //файловая переменная
  wp_format     format;                     //хранит инфу о формате звуковых данных
  unsigned int cnt;                        //для хранения количества прочитанных байт
  WPRESULT      res;

  res = wp_open(&file, FileName, &format);  //открыть файл для воспроизведения
  if(res) return WP_ERROR_FILE;             //ошибка открытия файла

  wp_init(&format);                         //настроить преобразователь согласно формату

  /*     Загрузить весь буфер ЦАПа данными из файла и запустить проигрывание     */
  if( f_read (&file, &DAC_Buff[0], SIZE_DAC_BUFF, &cnt) )
  {
    f_close(&file);                         //ошибка при чтении!!! Закрываем файл
    return WP_ERROR_READ_FILE;              //и выходим
  }

  WP_ENABLE                                 //запустить преобразование

  /*                  Воспроизведение файла                  */
  while(1)
  {
    while(!(DMA1->ISR & DMA_ISR_HTIF3));      //ждем освобождение первой части буфера
    DMA1->IFCR |= DMA_ISR_HTIF3;              //сбросить флаг
    //заполнить освободившуюся часть буфера
    res = f_read ( &file, &DAC_Buff[0], SIZE_DAC_BUFF/2, &cnt );
    if(res) {res = WP_ERROR_READ_FILE; break;}//ошибка при чтении!!!
    if(cnt<SIZE_DAC_BUFF/2) break;            //файл закончился

    while(!(DMA1->ISR & DMA_ISR_TCIF3));      //ждем освобождение второй части буфера
    DMA1->IFCR |= DMA_ISR_TCIF3;              //сбросить флаг
    //заполнить освободившуюся часть буфера
    res = f_read ( &file, &DAC_Buff[SIZE_DAC_BUFF/2], SIZE_DAC_BUFF/2, &cnt );
    if(res) {res = WP_ERROR_READ_FILE; break;}//ошибка при чтении!!!
    if(cnt<SIZE_DAC_BUFF/2) break;            //файл закончился

    //управление процессом воспроизведения
   // res = wp_control();
   // if(res!=WP_NONE)
   // {
   //   if(res==WP_PAUSE)                       //сделать паузу
   //   {
   //      WP_DISABLE;                          //остановить преобразование
   //      while(res!=WP_PLAY&&res!=WP_STOP)    //ожидать play или stop
   //      res = wp_control();
   //   }
   //   if(res==WP_STOP) break;                 //была команда стоп, закрываем файл
   //   WP_ENABLE;                              //возобновить преобразование
   // }
  }


  WP_DISABLE                                  //остановить преобразование
  f_close(&file);                             //закрыть файл перед выходом
  return res;                                 //возвратить результат

}
//********************************************************************************************
//function для управления процессом обмена                                                  //
//resultt  требуемое действие                                                               //
//********************************************************************************************
WPRESULT wp_control(void)
{
	/*Ндесь можно написать код для управления воспроизведением.
	  Например, при нажатии кнопки №1 вернуть WP_PAUSE
	            при нажатии кнопки №2 вернуть WP_PLAY
	            при нажатии кнопки №3 вернуть WP_STOP
	*/
//	if(get_key()==1) return WP_PAUSE;
//	if(get_key()==2) return WP_PLAY;
//	if(get_key()==3) return WP_STOP;
	return WP_NONE;
}
