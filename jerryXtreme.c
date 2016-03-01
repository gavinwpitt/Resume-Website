#define _BSD_SOURCE

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY 50000
#define HANGTIMEANDRECOVERY 6

/**
Prints a 'Jerry' to ride around on the scren
Given y and x coordinates mark the tail end of his board.
Jerry looks like: 
    _
   ( )  			
  /| |\                         
   |_|       
 __/_\__,
 ^(y,x)

@param y, an int that gives y coordinate of where to p rint Jerry
@param x, an int that gives x coordinate of where to print Jerry
@param stoke, measure Jerry's stoke level, and print him to represent as much.
Returns nothing
*/
void printJerry(int y, int x, int stoke){
	if(stoke == 0){
		mvprintw(y-4,x,"   _     ");
		mvprintw(y-3,x,"  (_)    ");
		mvprintw(y-2,x," /| |\\  ");
	}else{
		if(stoke > 0){
			if(stoke == 1){
				mvprintw(y-4,x,"'Sick!'     ");
				mvprintw(y-3,x," \\(+)    ");
				mvprintw(y-2,x,"  | |\\  ");
			}
			if(stoke == 2){
				mvprintw(y-5,x,"     _     ");
				mvprintw(y-4,x,"    (_)/   ");
				mvprintw(y-3,x,"   /| | ,  ");
				mvprintw(y-2,x,"    |_|_/  ");
				mvprintw(y-1,x,"      \\/  ");
				mvprintw(y  ,x,"      /    ");
				return;
			}
			if(stoke == 3){
				mvprintw(y-5,x,"     _     ");
				mvprintw(y-4,x,"   \\(_)   ");
				mvprintw(y-3,x,"    | |\\,  ");
				mvprintw(y-2,x,"  \\/|_|    ");
				mvprintw(y-1,x,"   \\/      ");
				mvprintw(y  ,x,"    \\,     ");		
				return;
			}
		}else{					
			mvprintw(y-4,x,"'BOGUS!'     ");
			mvprintw(y-3,x,"\\(--)/    ");
			mvprintw(y-2,x,"  | |  ");
		}
	}
	mvprintw(y-1,x,"  |_|    ");
	mvprintw(y  ,x,"__/_\\__,");
	//x value infront of tip of Jerry's Board
	return;
}

void printFloor(int y, int x){
	for(int i = 0; i < x; i++){
		mvprintw(y,i,"#");
	}
}

/**
Print a rail for Jerry to grind on!
Looks like this:
======
 |  |
^(y,x)

*/
void printRail(int y, int x){
	mvprintw(y-1,x,"=======");
	mvprintw(y  ,x," |   | ");
}

/**
  /
 /|
//|
#####
*/
void printRamp(int y, int x){
	mvprintw(y-2,x,"  /");
	mvprintw(y-1,x," /|");
	mvprintw(y-0,x,"/_|");
}
/**
main()
Initialize all values.
Run while loop that
	continualy checks for maximum x and y values of windows
	adjust variables depending on max x and y
	print the ground, and print Jerry
	check if user would like jerry to jump
	spawn rails for jerry to grind on
	check if jerry jumped onto rail, or if he crashed into it
	if he crashes into it, reset game
*/
int main(){
	int jerryY = 0, jerryX, jerryCenter, railX = 0;
	int max_y = 0, max_x = 0;
	int boardTip = 0;
	int topOfSnow;
	int jumpTimer = 0;
	int i = 0;
	int jumpDelay = 0;
	int stoke = 0;
	char input;
	
	//Initialization
	initscr(); //initialize curses mode
	noecho(); //turn off echoing
	cbreak();
	nodelay(stdscr, TRUE);
	curs_set(FALSE); //leave cursor visible
	getmaxyx(stdscr, max_y, max_x);
	jerryY = max_y - 2;
	jerryX = max_x / 8;

	while(1){
		getmaxyx(stdscr, max_y, max_x);
		topOfSnow = max_y -2;
		jerryX = max_x / 8;
		boardTip = jerryX + 8;
		jerryCenter = jerryX + 4;
		if(railX < 0){
			railX = max_x - 8;
		}
		if(jerryY != topOfSnow && stoke != 2){
			jerryY ++;
		}

		//jumpTimer will be set to 6 when spacebar is pressed
		if(jumpTimer > 0){
			jerryY = topOfSnow - 2;
			jumpTimer --;
			stoke = 1;
			//stop user from being able to spam jumps
			if(jumpTimer == 0){
				stoke = 0;
				jumpDelay = -HANGTIMEANDRECOVERY;
			}
		}else{
			if(mvinch(jerryY + 1,jerryX) == '=' || mvinch(jerryY + 1,boardTip) == '=' || mvinch(jerryY + 1,jerryCenter) == '='){
				stoke = 1;
				jerryY = topOfSnow - 2;
			}else{
				if(mvinch(jerryY,boardTip) == '/'){
					stoke = 2;
				}
				if(stoke == 2){
					jerryY --;
					if(mvinch(topOfSnow,jerryCenter - 2) == '|'){
						stoke = 0;
					}
				}			
			}		
		}
		
		//get User Input
		input = getch();
		//Handle SpaceBar Jump
		if(input == ' ' && jerryY == topOfSnow && jumpDelay == 0){
			jumpTimer = HANGTIMEANDRECOVERY;
			//Handle Jump this iteration
			jerryY = topOfSnow - 2;
			jumpTimer --;
		}

		//PrintStuff
		clear(); //Clear Screen
		printFloor(topOfSnow + 1, max_x);
		//printRail(topOfSnow, railX);
		printRamp(topOfSnow,railX);

		//Print jerry
		//printJerry(jerryY, jerryX, stoke); //print vall at current position 
		
		//Check for collisions

		//RAIL
		if(boardTip == 100 && jerryY == topOfSnow){
			mvprintw(jerryY - 6, jerryX, "YOU SUCK!");
			printJerry(jerryY, jerryX +1, -1);
			refresh();
			railX = max_x - 8;
			jerryY = topOfSnow;
			nodelay(stdscr, FALSE);
			getch();
			nodelay(stdscr, TRUE);			
		}

		//RAMP
		if(mvinch(jerryY,jerryCenter) == '_'){
			stoke = 2;
			//printJerry(jerryY ,jerryX, stoke);
		}else{
			//if(railX - jerryX >
			printJerry(jerryY,jerryX,stoke);
		}
		refresh();	
		

		i++;
		railX --;	
		//reset delay
		if(jumpDelay < 0){
			jumpDelay ++;
		}

		if(jerryY == topOfSnow){
			stoke = 0;	
		}
		
		usleep(DELAY);
			
	}

	endwin(); //end program
}
