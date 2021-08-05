/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../labyrinth/labyrinth.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/



void RIT_IRQHandler (void)
{			
	static int down=0;
	static int pressed1=0; 																																	/* No pressed button at beginning  											*/
	down++;                                                                                       
	                                                                                              
																																													/* Verify that KEY1 has been pressed 										*/
	                                                                                             
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 && !(LPC_PINCON->PINSEL4 & (1<<22))){             /* Button is pressed																		*/
		reset_RIT();                                                                                
		pressed1=1;                                                                                 																												
	}                                                                                       
	else {	/* button released */                                                                                                        
		if(!(LPC_PINCON->PINSEL4 & (1<<22)) && pressed1==1) { 																/* KEY1 has been released?															*/
			disable_timer(0);                                                                   /* Disable blinking 																		*/
			rotate_dir();																																				/* Function that rotate direction from labyrinth.h			*/
			LED_control();																																			/* Update variables for Timer0 (New direction blink)		*/
			enable_timer(0);    																																/* Re-enable blinking																		*/
			
			/* Implementing debouncing. This code will be executed just 1 time due to pressed1 variable */
			down=0;			
			pressed1=0;
			disable_RIT();
			reset_RIT();	
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}else if(!(LPC_PINCON->PINSEL4 & (1<<22))) {	
			/* Bouncing could make button interrupt function called multiple time and disable IRQ, so it must be enabled everytime bouncing disable it */
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0 && !(LPC_PINCON->PINSEL4 & (1<<24))){ 					 /* Verify that KEY2has been pressed 										*/
		reset_RIT();
		if(down==1) { 																					/* When button is pressed, enable RUNNING_LED but do not move until 1s has passed 	*/
			LED_On(RUN_LED);
			enable_timer(2);																			/* Timer for RUNNING_LED blinking 																									*/
		}
		if(down==11){ 																					/* (10*100ms = 1s has passed after pressing 																				*/
				move_robot();																				/* Function that moves robot in direction 																					*/
				LED_control();																			/* Verify if the new position has obstacles near 																		*/
				down=1;																							/* Reset counter																																		*/
				LED_On(RUN_LED);												
				enable_timer(2);   
		}
	}
	else {	/* button released */
		if(!(LPC_PINCON->PINSEL4 & (1<<24))) {									/* KEY2 has been released?																													*/
			disable_timer(2);																			/* RUNNING_LED must be stopped if not moving																				*/
			LED_Off(RUN_LED);				
			
			/* De-bouncing implementation */
			down=0;
			disable_RIT();
			reset_RIT();
			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
