#ifndef _I2Croutines_h
#define _I2Croutines_h
#include "stm32f10x.h"
#include "defines.h"

#define FPANEL_CMD_GET_STATUS                   0x00
#define FPANEL_CMD_GET_EVENTS                   0x01
#define FPANEL_CMD_LIGHT                        0x02
#define FPANEL_CMD_LEDS                         0x03
#define FPANEL_CMD_GET_DISPLAY_CONFIG           0x04

//GET_DISPLAY_CONFIG
#define FPANEL_MSK_DISPLAY_TYPE			0x0Fu
#define FPANEL_MSK_BUS_WIDTH			0x10u
#define FPANEL_MSK_CONTROLLER_TYPE		0x60u

//Constants
//DISPLAY_TYPE
#define FPANEL_DISPLAY_320x240			0x01u
#define FPANEL_DISPLAY_480x272_LQ43		0x02u
#define FPANEL_DISPLAY_800x480_LM50		0x03u
//CONTROLLER_BUS_WIDTH
#define FPANEL_BUS_8BIT				0x00u
#define FPANEL_BUS_16BIT			0x10u

//CONTROLLER_TYPE
#define FPANEL_CONTROLLER_ILI9341		0x00u
#define FPANEL_CONTROLLER_SSD1963		0x20u



extern u8 Status;


void ConfigI2C(void);
void ProcessCommand(u8* pCommand);
void SolveCommand(u8 Command);
#endif
