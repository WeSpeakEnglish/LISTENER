########################################################################
#
#                           LCD_Demo.eww
#
# $Revision: 34964 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR STM32-SK board. It shows basic
  use of the parallel I/O, the timer, the interrupt controller, the ADC and the
  interface to LCD HD44780 compatible module.
   It show the position of on the board potentiometer in percents.

COMPATIBILITY
=============
   The LCD_Demo example project is compatible with IAR-STM32-SK
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   Make sure that the following jumpers are correctly configured on the
  IAR STM32-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source
        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\ST\
     STM32F10x\IAR-STM32-SK\LCD_Demo\LCD_Demo.eww

  3) Run the program.

     The LCD Demo application is downloaded to the Embedded RAM or 
     Embedded Flash memory depending of configurations on the evaluation 
     board and executed.
