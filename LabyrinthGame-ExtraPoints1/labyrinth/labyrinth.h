#ifndef __LABYRINTH_H
#define __LABYRINTH_H

/* Constants to refer LED numbers to direction/running */
#define NORD 3
#define EST 2
#define SUD 1
#define WEST 0
#define RUN_LED 5

/* Constants for starting position and labirinth definition */
#define START_POSX 7
#define START_POSY 7
#define START_DIR 'E'
#define YMAX 13
#define XMAX 15

#define RANGE_SENSOR 7 	// It will consider 5 SLOTS of distance

static char lab[YMAX][XMAX]={
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {1,1,1,1,1,0,0,1,0,1,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {2,0,1,0,0,0,0,0,0,0,0,0,0,0,2} 
};

extern int posX, posY;
extern char dir;
extern int start;

extern int distance;
extern int obstacle;
extern int victory;

void rotate_dir(void);
void move_robot(void);
void LED_control(void);
void checkVictory(void);
#endif /* end __LABYRINTH_H */
