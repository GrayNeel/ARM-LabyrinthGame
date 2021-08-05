/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../labyrinth/labyrinth.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/


void TIMER0_IRQHandler (void)
{
	static int blink = 0;
	static int on = 1;
	static int victoryBlink = 0;
	static int vic_on = 0;
	int dirNum;
	
	if(victory==1){																										/* If the victory point has been reached																		*/
		victoryBlink++;																									/* Variable that implement 1 Hz blinking, because timer has 16Hz frequency 	*/
		if(victoryBlink%8 == 0){ //1 HZ
			if(!vic_on) {
				LED_On(RUN_LED);
				LED_On(EST);
				LED_On(WEST);
				LED_On(NORD);
				LED_On(SUD);
				vic_on=1;
			}else{
				LED_Off(RUN_LED);
				LED_Off(EST);
				LED_Off(WEST);
				LED_Off(NORD);
				LED_Off(SUD);
				vic_on=0;
			}
		}
	}else	if(obstacle==1){ 																					/* No victory but obstacle 																									 */
		blink++;
		
		switch(dir) { 																	/* dirNum will tell which LED must blink																					 				 */
			case 'E': 
				dirNum=EST;
				break;
			case 'S': 
				dirNum=SUD;
				break;
			case 'W':
				dirNum=WEST;
				break;
			case 'N':
				dirNum=NORD;
				break;
		}
		
		switch(distance) { 																											/* Based on distance, leds must blink at different frequency 			*/
			case 1:
				//ROBOT RIGHT IN FRONT OF THE OBSTACLE  (0 SLOT DISTANCE)
				//8HZ DIR LED BLINKING
				if(blink%2 != 0) {	
						LED_Off(dirNum);
				}else{
						LED_On(dirNum);
				}
				break;
			case 2:
			case 3:
				//ROBOT 1-2 SLOT DISTANCE
				//4HZ DIR LED BLINKING
				if(blink%2 == 0){
					if(!on) {
						LED_On(dirNum);
						on=1;
					}else{
						LED_Off(dirNum);
						on=0;
					}
				}
				break;
			case 4:
			case 5: 
			case 6:  
				//ROBOT 3-4-5 SLOT DISTANCE
				//2HZ DIR LED BLINKING
				if(blink%4 == 0){
					if(!on) {
						LED_On(dirNum);
						on=1;
					}else{
						LED_Off(dirNum);
						on=0;
					}
				}
				break;
			default:
				break;
	}
	
	if(blink==16) {
		blink=0;
	}
	
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	static int obstacleBlink = 0;																				 /* Variable for 5 Hz blinking frequency in case of RUNNING_LED obstacles */
	static int on = 1;
	static int blink = 0; 																							 /* 1Hz blinking 																													*/
	blink++;
	
	if(obstacle==1 && distance==1){ 																		 /* 5Hz blinking in case of obstacle in the front 												*/
		obstacleBlink++;
		
		if(obstacleBlink%2 != 0) {		
			LED_On(RUN_LED);
		}else{
			LED_Off(RUN_LED);
		}
			
		if(obstacleBlink==10) {
			obstacleBlink=0;
		}
	
		if(blink==10){
			blink=0;
		}
	}else{
		/* 1Hz blinking when moving */
		if(blink%5 == 0){
			if(!on) {
				LED_On(RUN_LED);
				on=1;
			}else{
				LED_Off(RUN_LED);
				on=0;
			}
		}
	}
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
