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

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;

void RIT_IRQHandler (void)
{					
	static int select=0;
	static int south=0;
	static int north=0;
	static int west=0;
	static int east=0;
	//reset_RIT(); 	ogni tanto il RIT fa le bizze e non resetta il counter: conviene farlo esplicitamente
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0 && !victory){	
		/* Joystick Select pressed */
		select++;
		switch(select){
			case 1:
				if(mode=='E')
					mode='M';
				else if(mode=='M')
					mode='E';
				clear_robot(posX, posY);
				draw_robot(posX, posY, dir);
				break;
			default:
				break;
		}
		
		/* AZZERAMENTO DELLE VARIABILI PER EVITARE MOVIMENTI PRECOCI IN CASO DI MODIFICA DELLA MODALITA' DA EXPLORE A MOVE QUANDO UNA DIREZIONE E' GIA' SELEZIONATA TRAMITE JOYSTICK */
		//ES: direzione nord selezionata (nord++) e pressione di select dopo mezzo secondo (il contatore non si azzererebbe)
		south=0;
		north=0;
		west=0;
		east=0;
	}
	else{
			select=0;
	}
	
	/*DOWN*/
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && !victory  && (LPC_GPIO1->FIOPIN & (1<<27))  && (LPC_GPIO1->FIOPIN & (1<<28))){	
		/* Joystick Down Button pressed */
		
		/*EXPLORE FATTA SOLTANTO LA PRIMA VOLTA CHE LA LEVETTA VIENE INCLINATA VERSO SUD */
		if(south==0)
			robot_explore('S');
		
		south++;
		switch(south){
			case 40:
				if(mode=='M') {
					move_robot();
					south=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			south=0;
	}
	
	/*WEST*/
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && !victory && (LPC_GPIO1->FIOPIN & (1<<26))  && (LPC_GPIO1->FIOPIN & (1<<29))){	
		/* Joystick West Button pressed */
		
		/*EXPLORE FATTA SOLTANTO LA PRIMA VOLTA CHE LA LEVETTA VIENE INCLINATA VERSO OVEST */
		if(west==0)
			robot_explore('W');
		
		west++;
		switch(west){
			case 40:
				if(mode=='M') {
					move_robot();
					west=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			west=0;
	}
	
	/*EAST*/
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && !victory && (LPC_GPIO1->FIOPIN & (1<<29))  && (LPC_GPIO1->FIOPIN & (1<<26))){	
		/* Joystick East Button pressed */
		
		/*EXPLORE FATTA SOLTANTO LA PRIMA VOLTA CHE LA LEVETTA VIENE INCLINATA VERSO EST */
		if(east==0)
			robot_explore('E');
		
		east++;
		switch(east){
			case 40:
				if(mode=='M') {
					move_robot();
					east=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			east=0;
	}
	
	/*NORTH*/
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0 && !victory && (LPC_GPIO1->FIOPIN & (1<<27))  && (LPC_GPIO1->FIOPIN & (1<<28))){	
		/* Joystick North Button pressed */
		
		/*EXPLORE FATTA SOLTANTO LA PRIMA VOLTA CHE LA LEVETTA VIENE INCLINATA VERSO NORD */
		if(north==0)
			robot_explore('N');
		
		north++;
		switch(north){
			case 40:
				if(mode=='M') {
					move_robot();
					north=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			north=0;
	}
	
	
//	/* button management */
//	if(down!=0){ 
//		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
//			down++;				
//			switch(down){
//				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
//				if( position == 7){
//					LED_On(0);
//					LED_Off(7);
//					position = 0;
//				}
//				else{
//					LED_Off(position);
//					LED_On(++position);
//				}
//					break;
//				default:
//					break;
//			}
//		}
//		else {	/* button released */
//			down=0;			
//			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
//			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
//		}
//	}
	//reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
