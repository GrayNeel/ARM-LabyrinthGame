#ifndef __LABYRINTH_H
#define __LABYRINTH_H

#define NORD 3
#define EST 2
#define SUD 1
#define WEST 0
#define START_POSX 7
#define START_POSY 7
#define START_DIR 'E'
#define YMAX 13
#define XMAX 15

#define SCALE_FACTOR 14

#define PANEL_X0 15
#define PANEL_X1 225
#define PANEL_Y0 50
#define PANEL_Y1 232

#define RESET_BUTTON_X0 15
#define RESET_BUTTON_Y0 259
#define RESET_BUTTON_X1 105
#define RESET_BUTTON_Y1 295


#define CLEAR_BUTTON_X0 135
#define CLEAR_BUTTON_Y0 259
#define CLEAR_BUTTON_X1 225
#define CLEAR_BUTTON_Y1 295

#define ROBOT_EXPLORE_COLOR Magenta
#define ROBOT_MOVE_COLOR Green
#define OBSTACLE_COLOR Red
#define RESET_TEXT_COLOR Blue
#define PANEL_COLOR Grey
#define TEXT_BUTTON_COLOR Blue
#define BUTTON_COLOR Grey
#define VICTORY_TEXT_COLOR Magenta 
#define VICTORY_BG_COLOR Cyan

#define RANGE_SENSOR 6 	//5 SLOT partendo dalla casella davanti (+1)

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
typedef	struct PUNTO 
{
	 int x;
   int y;
}PixelCoord;

extern int posX, posY;
extern char dir;
extern int start;

extern int distance;
extern int obstacle;
extern int victory;
extern int restart;

extern char mode;

void init_labyrinth(void);
void draw_panel(void);
void draw_title(void);
void draw_grid(void);
void draw_buttons(void);
PixelCoord coord_transform(int x, int y);
void draw_obstacle(int x,int y);
void reset_labyrinth(void);
void start_labyrinth(void);
void draw_robot(int x, int y, char dir);
void clear_robot(int x, int y);
void clear_labyrinth(void);
void robot_explore(char joydir);
void move_robot(void);
void obstacle_scan(void);
void checkVictory(void);

#endif /* end __LABYRINTH_H */
