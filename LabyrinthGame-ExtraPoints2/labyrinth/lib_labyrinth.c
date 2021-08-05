#include <stdio.h>
#include "LPC17xx.H"                    
#include "labyrinth.h"
#include "../GLCD/Asciilib.h"
#include "../GLCD/GLCD.h"
#include "../joystick/joystick.h"
#include "../RIT/RIT.h"



int distance; //la distanza dell'ostacolo davanti a me
int obstacle; //obstacle e' 1 se davanti a me c'e' un ostacolo entro il range del sensore
int victory;
int restart;

int posX,posY;
char dir;

char mode='E';		//ROBOT MODE

void init_labyrinth(void){
	LCD_Clear(Blue);
	draw_title();
	draw_buttons();
	reset_labyrinth();
}

void reset_labyrinth(void){
	draw_panel();
	//draw_grid();
	GUI_Text(65, 125, (uint8_t *) "Touch to start", RESET_TEXT_COLOR, PANEL_COLOR);
	GUI_Text(81, 141, (uint8_t *) "a new game", RESET_TEXT_COLOR, PANEL_COLOR);
	//TODO: Inizializzare il labirinto (modalità, posizione, direzione robot)
	restart=1;
}

void clear_labyrinth(void){
	draw_panel();
	draw_grid();
	draw_robot(posX, posY, dir);
}

void start_labyrinth(void) {
	
	draw_panel();
	draw_grid();
	posX=7;
	posY=7;
	dir='E';
	mode='M';
	victory=0;
	enable_RIT();
	//WRITE ROBOT IN STARTING POSITION (7,7) EST DIRECTION
	draw_robot(START_POSX, START_POSY, dir);
}

void draw_robot(int x, int y, char dir) {
	PixelCoord pos;
	
	pos=coord_transform(x,y);
	
	switch(mode){
		case 'E':
			LCD_DrawTriangle(pos.x+2,pos.y+2,10,ROBOT_EXPLORE_COLOR,dir);	
			break;
		case 'M':
			LCD_DrawTriangle(pos.x+2,pos.y+2,10,ROBOT_MOVE_COLOR,dir);	
			break;
		default:
			break;
	}
}

void clear_robot(int x, int y) {
	PixelCoord pos;
	
	pos=coord_transform(x,y);
	LCD_DrawRectangle(pos.x+1,pos.y+1,pos.x+SCALE_FACTOR-1,pos.y+SCALE_FACTOR-1,PANEL_COLOR);
}

void draw_panel(void){
	LCD_DrawRectangle(PANEL_X0,PANEL_Y0,PANEL_X1,PANEL_Y1,PANEL_COLOR);

}
void draw_title(void){
	GUI_Text(65, 20, (uint8_t *) "Blind labyrinth", Blue, White);
	GUI_Text(58, 36, (uint8_t *) "V A P O R W A V E", Green, Blue);
}
void draw_grid(void){
	int i=0;
	for(i=PANEL_X0; i<=PANEL_X1; i+=14)
		LCD_DrawLine(i,PANEL_Y0,i,PANEL_Y1,Black);
	for(i=PANEL_Y0; i<=PANEL_Y1; i+=14)
		LCD_DrawLine(PANEL_X0,i,PANEL_X1,i,Black);
	
}

/**
* @param Coordinate della posizione nel labirinto in forma (PosX, PosY) (es. (4,3))
* @return Disegno dell'ostacolo del display
*/
void draw_obstacle(int x,int y){
	PixelCoord pos;
	pos=coord_transform(x,y);
	LCD_DrawRectangle(pos.x,pos.y,pos.x+SCALE_FACTOR,pos.y+SCALE_FACTOR,OBSTACLE_COLOR);
}

void draw_buttons(void){
	LCD_DrawRectangle(RESET_BUTTON_X0,RESET_BUTTON_Y0,RESET_BUTTON_X1,RESET_BUTTON_Y1,BUTTON_COLOR);
	LCD_DrawRectangle(CLEAR_BUTTON_X0,CLEAR_BUTTON_Y0,CLEAR_BUTTON_X1,CLEAR_BUTTON_Y1,BUTTON_COLOR);
	GUI_Text(32, 269, (uint8_t *) "RESTART", TEXT_BUTTON_COLOR, BUTTON_COLOR);
	GUI_Text(160, 269, (uint8_t *) "CLEAR", TEXT_BUTTON_COLOR, BUTTON_COLOR);
	
}
/**
* @param Coordinate della posizione nel labirinto in forma (PosX, PosY) (es. (4,3))
* @return Coordinate scalate nella griglia 320x240 del display (Pixel in alto a sx)
*/
PixelCoord coord_transform(int x, int y) {
	PixelCoord point;
	point.x = x*SCALE_FACTOR+PANEL_X0;
	point.y = y*SCALE_FACTOR+PANEL_Y0;
	
	return point;
}

void move_robot(void){
	clear_robot(posX,posY);
	switch(dir) {
		case 'E':
			if(posX+1<XMAX && lab[posY][posX+1]!=1)   // IF THE NEXT POSITION IS NOT PERIMETER AND AN OBSTACLE
				posX++;																 	// MOVE IN THE DIRECTION "EST"
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
	draw_robot(posX,posY,dir);
	obstacle_scan();
	checkVictory();
	return;
}

void robot_explore(char joydir) {
	
	if(joydir!=dir) {
		dir=joydir;
		clear_robot(posX, posY);
		draw_robot(posX, posY, dir);
	}
	
	obstacle_scan();

}

void obstacle_scan(void) {
	int i;
	
	obstacle=0;
	
	if(lab[posY][posX] == 2)
		return; //se sono sulla vittoria non devo controllare gli ostacoli
	
	switch(dir) {
		case 'E':
			for(i=1; i<RANGE_SENSOR && posX+i<XMAX && !obstacle; i++) {
				if(lab[posY][posX+i] == 1) {
					draw_obstacle(posX+i,posY);
					obstacle=1; //ho un ostacolo 
				}
			}
			break;
		case 'S':
			for(i=1; i<RANGE_SENSOR && posY+i<YMAX && !obstacle; i++) {
				if(lab[posY+i][posX] == 1) {
					draw_obstacle(posX,posY+i);
					obstacle=1;
				}
			}
		  break;
		case 'W':
			for(i=0; i<RANGE_SENSOR && posX-i>=0 && !obstacle; i++) {
				if(lab[posY][posX-i] == 1) {
					draw_obstacle(posX-i,posY);
					obstacle=1;
				}
			}
			break;				
		case 'N':
			for(i=0; i<RANGE_SENSOR && posY-i>=0 && !obstacle; i++) {
				if(lab[posY-i][posX] == 1) {
					draw_obstacle(posX,posY-i);
					obstacle=1;
				}
			}
			break;
	}
	
	return;
}

void checkVictory(void) { //check sulla vittoria
	victory=0;
	if(lab[posY][posX] == 2){ 
		//se mi trovo sulla vittoria
		//disabilito tutto tranne il timer1 che utilizzero' per il "lampeggio della vittoria"
		victory=1;
		//joystick_disable();
		reset_RIT();
		disable_RIT();
		//draw_panel();
		GUI_Text(88, 125, (uint8_t *) "VICTORY!", VICTORY_TEXT_COLOR, VICTORY_BG_COLOR);
		GUI_Text(32, 141, (uint8_t *) "Press Restart to begin", VICTORY_TEXT_COLOR, VICTORY_BG_COLOR);
		GUI_Text(80, 157, (uint8_t *) "a new game", VICTORY_TEXT_COLOR, VICTORY_BG_COLOR);
	}
}
