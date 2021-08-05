/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: Control a robot inside a labyrinth by using buttons and LEDs
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Marco Smorti - S281554 
 *					Mattia Riola - S280169
 *					Gabriele Scaffidi Militone - S288200
 *					last modified 08/01/2020 - Group authorized by Prof. Ernesto Sanchez
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2021 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"

/* Local variables that identify the position and direction of the robot			*/
int posX, posY;
char dir;
int start;						 /* Boolean variable that check if the game has started */

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main (void) {	
	SystemInit();  												/* System Initialization (i.e., PLL)           					*/
  LED_init();                           /* LED Initialization                         				  */
  BUTTON_init();												/* BUTTON Initialization             				  				  */
	init_RIT(0x00989680);									/* RIT Initialization counter = 100msec*100MHz (RITCLK) */
	init_timer(0,0x000B71B0);							/* Enable Timer0 with counter = 62.5 msec*12MHzcount		*/
	init_timer(2,0x00124F80);							/* Enable Timer2 with counter = 0.1sec*12MHz						*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										         					*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* loop forever																					*/
		__ASM("wfi");
  }

}
