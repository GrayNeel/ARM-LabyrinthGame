/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
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

void TIMER0_IRQHandler (void)
{
	getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
	
	if(restart==1){	
		if(display.x <= PANEL_X1 && display.x > PANEL_X0){
			if(display.y <= PANEL_Y1 && display.y > PANEL_Y0){
				restart=0;
				start_labyrinth();
			}
		}
		else{
		//do nothing if touch returns values out of bounds
		}
	}else{
		//IS NOT RESTART CASE, SO WE MANAGE RESET AND CLEAR BUTTONS
		
		
		//RESET BUTTON
		if(display.x <= RESET_BUTTON_X1 && display.x > RESET_BUTTON_X0){
			if(display.y <= RESET_BUTTON_Y1 && display.y > RESET_BUTTON_Y0){
				reset_labyrinth();
			}
		}	
		
		//CLEAR BUTTON
		if(display.x <= CLEAR_BUTTON_X1 && display.x > CLEAR_BUTTON_X0){
			if(display.y <= CLEAR_BUTTON_Y1 && display.y > CLEAR_BUTTON_Y0){
				if(victory==0)
					clear_labyrinth();
			}
		}	
		
	}
	
  
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
