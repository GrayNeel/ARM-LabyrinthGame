#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h" 					
#include "../RIT/RIT.h"		  		
#include "../labyrinth/labyrinth.h"
#include "../timer/timer.h"


void EINT0_IRQHandler (void)	  	/* INT0							inizio gioco (no debouncing)							 */
{		
	/********************************************************************
	 *	Per scelta di progetto non ho bisogno del debouncing sullo start *
	 *  perché non avrei effetti per pressioni multiple di questo tasto  *
	********************************************************************/
	
	if(!start) { 											/* If the game has not been started, then it will begin by initializing all variables and timers */
		LED_On(EST);										/* EST Led is ON at the beginning  																															 */  
		start=1;                        /* Disable future pressing of the INT0 button 																									 */
		enable_timer(0); 					      /* Starting position of the robot: (7,7) EST 																										 */
		posX=START_POSX;
		posY=START_POSY;
		dir=START_DIR;
		LED_control();									/* Timer that kill make LEDs blink in case of obstacle/victory 																	 */
	}
	
	LPC_SC->EXTINT &= (1 << 0);    		/* clear pending interrupt     																															     */
}


void EINT1_IRQHandler (void)	  		/* KEY1																																													 */
{
	if(!start) {											/* If the game has not started, then the button must be disabled 																 */
		LPC_SC->EXTINT &= (1 << 1);
		return;
	}
	
	enable_RIT();											/* enable RIT to count 100ms																																		 */
	NVIC_DisableIRQ(EINT1_IRQn);			/* disable Button interrupts																																		 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection                                                                    */
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 																																 */
{                                 
	if(!start) {                    /* If the game has not started, then the button must be disabled 																	 */
		LPC_SC->EXTINT &= (1 << 2);
		return;
	}
	
	enable_RIT();										/* enable RIT to count 100ms																																			 */
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts																																			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection 																																	 */
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt      																																   */    
}


