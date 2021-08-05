#include <stdio.h>
#include "LPC17xx.H"                    
#include "../led/led.h"
#include "labyrinth.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"

/* Variable that tell which is the distance of the obstacle (if present) or if the player has reached the exit */
int distance; 
int obstacle; 
int victory;

/* Function that rotate the robot and turns on the new direction LED */
void rotate_dir(void){
			switch(dir) {
					case 'E': 
						dir='S';
						LED_Off(EST);
						LED_On(SUD);
						break;
					case 'S': 
						dir='W';
						LED_Off(SUD);
						LED_On(WEST);
						break;
					case 'W':
						dir='N';
						LED_Off(WEST);
						LED_On(NORD);
						break;
					case 'N':
						dir='E';
						LED_Off(NORD);
						LED_On(EST);
						break;
				}
	return;
}

/* Functions that moves the robot if there's no obstacle and no perimeter limit */
void move_robot(void){
	switch(dir) {
		case 'E':
			if(posX+1<XMAX && lab[posY][posX+1]!=1)  // IF THE NEXT POSITION IS NOT PERIMETER AND AN OBSTACLE
				posX++;																 // MOVE IN THE DIRECTION "EST"
			break;
		case 'S':
			if(posY+1<YMAX && lab[posY+1][posX]!=1)
				posY++;
			break;
		case 'W':
			if(posX-1>=0 && lab[posY][posX-1]!=1)
				posX--;
			break;
		case 'N':
			if(posY-1>=0 && lab[posY-1][posX]!=1)
				posY--;
			break;
	}
	checkVictory();						/* Check for victory */
	return;
}

void LED_control(void) {
	int i,foundexit=0; /* Foundexit is needed to DO NOT make LED blinking in case of exit near to the robot */ 
	obstacle=0;
	distance=RANGE_SENSOR; /* If distance is not modified due to obstacle/perimeter/victory, then i suppose max distance */
	if(lab[posY][posX] == 2)	/* If i win i do not need to check for obstacles 																					 */
		return; 
	
	switch(dir) {
		case 'E':
			for(i=1; i<RANGE_SENSOR && posX+i<XMAX && !obstacle; i++) {
				if(lab[posY][posX+i] == 1) {
					distance=i;
					obstacle=1;  
				}
				if(lab[posY][posX+i] == 2) 
					foundexit=1;
			}
			
			if(i<RANGE_SENSOR && !foundexit && !obstacle) {
				distance=i;
				obstacle=1;
			}
			break;
		case 'S':
			for(i=1; i<RANGE_SENSOR && posY+i<YMAX && !obstacle; i++) {
				if(lab[posY+i][posX] == 1) {
					distance=i;	
					obstacle=1;
				}
				if(lab[posY+i][posX] == 2)
					foundexit=1;
			}
				
			if(i<RANGE_SENSOR && !foundexit && !obstacle) {
				distance=i;
				obstacle=1;
			}
		  break;
		case 'W':
			for(i=0; i<RANGE_SENSOR && posX-i>=0 && !obstacle; i++) {
				if(lab[posY][posX-i] == 1) {
					distance=i;
					obstacle=1;
				}
				if(lab[posY][posX-i] == 2) 
					foundexit=1;
			}
			
			if(i<RANGE_SENSOR && !foundexit && !obstacle) {
				distance=i;
				obstacle=1;
			}
			break;				
		case 'N':
			for(i=0; i<RANGE_SENSOR && posY-i>=0 && !obstacle; i++) {
				if(lab[posY-i][posX] == 1) {
					distance=i;
					obstacle=1;
				}
				if(lab[posY-i][posX] == 2)
					foundexit=1;
			}
				
			if(i<RANGE_SENSOR && !foundexit && !obstacle) {
				distance=i;
				obstacle=1;
			}
			break;
	}
	
	return;
}


void checkVictory(void) { 																			/* Victory check */
	victory=0;
	if(lab[posY][posX] == 2){ 
		disable_timer(2);																						/* If player win disable everything except for timer0 that will make led blink at 1Hz */
		NVIC_DisableIRQ(EINT0_IRQn);																/* Disable button interrupt	*/
		NVIC_DisableIRQ(EINT1_IRQn);
		NVIC_DisableIRQ(EINT2_IRQn);
		disable_RIT();
		/* Set victory variable to declare system that player won and make timer0 blink LEDs correctly */
		victory=1;
	}
}
