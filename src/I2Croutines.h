#ifndef _I2Croutines_h
#define _I2Croutines_h
#include "stm32f10x.h"

#define FPANEL_CMD_GET_STATUS                   0x00
#define FPANEL_CMD_GET_EVENTS                   0x01
#define FPANEL_CMD_LIGHT                        0x02
#define FPANEL_CMD_LEDS                         0x03
#define FPANEL_CMD_GET_DISPLAY_CONFIG           0x04

//GET_DISPLAY_CONFIG
#define FPANEL_MSK_DISPLAY_TYPE			0x0F
#define FPANEL_MSK_BUS_WIDTH			0x10
#define FPANEL_MSK_CONTROLLER_TYPE		0x60

//Constants
//DISPLAY_TYPE
#define FPANEL_DISPLAY_320x240			0x01
#define FPANEL_DISPLAY_480x272_LQ43		0x02
#define FPANEL_DISPLAY_800x480_LM50		0x03
//CONTROLLER_BUS_WIDTH
#define FPANEL_BUS_8BIT				0x00
#define FPANEL_BUS_16BIT			0x10

//CONTROLLER_TYPE
#define FPANEL_CONTROLLER_ILI9341		0x00
#define FPANEL_CONTROLLER_SSD1963		0x20



extern u8 Status;

#define DISP_CONFIG FPANEL_DISPLAY_320x240+FPANEL_BUS_8BIT+FPANEL_CONTROLLER_ILI9341


void ConfigI2C(void);
void SolveCommand(u8);
void ProcessCommand(u8* pCommand);
#endif