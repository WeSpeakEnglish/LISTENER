#ifndef _adc_h
#define _adc_h

void ADC_config(void);
void AD_SetTimer(uint16_t prescaler, uint16_t period);
u8 AD_CheckVoltages(void);
void GenerateCheck_V_Msg(void);
extern u8 ADCdataReady;
extern u8 CheckVoltagesError;

struct VoltErr{
  u8 V3Err         : 1;
  u8 V5Err         : 1;
  u8 Vplus5Err     : 1;
  u8 Vminus5Err    : 1;
  u8 V12Err        : 1;
  u8 Vplus15Err    : 1;
  u8 Vminus15Err   : 1;
  u8 VisoErr       : 1;
};

extern u16 ADC_VAL[16];
// analog voltages have 5% accuracy
// digital ones have only 10% accuracy
#define plus_3V_low_limit      2970000 // 0.9*(3.3*10^6) uV
#define plus_3V_high_limit     3630000 // 1.1*(3.3*10^6) uV
#define plus_5V_low_limit      2250000 // 0.9*(5*10^6)*1.89/(1.89+1.89) uV
#define plus_5V_high_limit     2750000 // 1.1*(5*10^6)*1.89/(1.89+1.89) uV
#define plus_5VA_low_limit     2375000 // 0.95*(5*10^6)*1.89/(1.89+1.89) uV
#define plus_5VA_high_limit    2625000 // 1.05*(5*10^6)*1.89/(1.89+1.89) uV
#define minus_5VA_high_limit   1500000 // (-5*0.95+10*10.2/(6.12+10.2))*10^6 uV
#define minus_5VA_low_limit    1000000 // (-5*1.05+10*10.2/(6.12+10.2))*10^6 uV
#define plus_12V_low_limit     2250000 // 0.9*(12*10^6)*1.15/(4.37+1.15) uV
#define plus_12V_high_limit    2750000 // 1.1*(12*10^6)*1.15/(4.37+1.15) uV
#define minus_15VA_high_limit  2000000 // ((-15*0.95+20*10.4/(2.4+10.4))*10^6 uV
#define minus_15VA_low_limit   500000  //  (-15*1.05+20*10.4/(2.4+10.4))*10^6 uV
#define plus_15VA_high_limit   2625000  //  1.05*(15*10^6)*1.01/(5.05+1.01) uV
#define plus_15VA_low_limit    2375000  //  0.95*(15*10^6)*1.01/(5.05+1.01) uV

#endif


