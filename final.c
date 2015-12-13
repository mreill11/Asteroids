/* asteroids.c
 * Matt Reilly and Chris Beaufils
 * This is our final project, a game we created called Dodge the Asteroid.
 * It is inspired by the movement in the game Asteroids.
 * Control the ship using the arrow keys.
 * We are using the C language and the gfx library for graphics.
 */

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "gfx4.h"
#define DELTAT 10000

void setupWindow(int xSize, int ySize);
void drawShip(int xPos, int yPos, double dir);
int endGame(int *game, int *gL, int *n);
void newLife(int *cX, int *cY, int *cXD, int *cYD, int *sS, int *x, int *y);
void adjustLivesTicker(int n);
void wrapShipAround(int *x, int *y);
int varyRadius(int c, int j, int r);

int main() {
	int gameLive = 1; 	//boolean to describe game state
	int *gL = &gameLive; 	//pointer to ^
	setupWindow(350, 350);
	int xSize = 350, ySize = 350, i = 1, shipSpeed = 0; 	//defining initial vars
	int *game = &i; 	//pointer to variable that keeps while loop going
	int *sS = &shipSpeed; 	//pointer to ship speed
	int circleX = (rand() % 350); 	//circle x location
	int *cX = &circleX; 		//pointer to ^
	int circleY = (rand() % 350); 	//circle y location
	int *cY = &circleY; 		//pointer to ^
	int circleXDirection = (rand() % 12) - 6, circleYDirection = (rand() % 12) - 6; //defining circle init direction
	int *cXD = &circleXDirection, *cYD = &circleYDirection; 			//pointers to ^
	int radius = 12; 		//radius of circle
	int count = 1, j = 0, numLives = 3; 	//various vars for iteration
	int *n = &numLives; 			//pointer to numLives

	//make sure circle doesn't start on top of ship
	if (abs(circleX - 175) < 15 && abs(circleY - 175) < 15)
		circleX = 35;

	//Initialize vars for gameplay
	double dir = (M_PI) / 2;
	char c;
	double angle = .75;
	int shipXPos = 175, shipYPos = 175;

	int *x = &shipXPos, *y = &shipYPos; 	//pointers to ship location
	
	drawShip(xSize / 2, ySize / 2, dir); 	//place ship in middle of screen
	gfx_circle(circleX, circleY, radius); 	//draw circle

	while (i) {
		gfx_clear();
		
		adjustLivesTicker(numLives); 	//display lives counter
		
		circleX += circleXDirection; 	//move circle
		circleY += circleYDirection;

		gfx_circle(circleX, circleY, radius);
		
		radius = varyRadius(count, j, radius); //handle radius changes
		j++;
		
		//bounce circle off walls
		if (circleX - radius <= 0 || circleX + radius >= xSize)
			circleXDirection = -circleXDirection;
		if (circleY - radius <= 0 || circleY + radius >= ySize)
			circleYDirection = -circleYDirection;
		
		drawShip(shipXPos, shipYPos, dir); //draw ship
		
		wrapShipAround(x, y); 			// make ship wrap around when it leaves the screen
		
		//handle ship / circle collisions
		if (shipXPos > (circleX - radius) && shipXPos < (circleX + radius) && shipYPos > (circleY - radius) && shipYPos < (circleY + radius)) {
			usleep(1000);
			numLives--; 	//decrease numLives
			if (numLives < 1) {
				gameLive = endGame(game, gL, n); 	//figure out if user wants another game
				if (gameLive)
					newLife(cX, cY, cXD, cYD, sS, x, y); //start new lives if they do
			} else {
				gfx_text(150, 30, "Press space to continue");
				int k = 1;
				while (k) {
					char c = gfx_wait();
					if (c == ' ') { //start new life when user is ready
						newLife(cX, cY, cXD, cYD, sS, x, y);
						k = 0;
					} 
				}
			}
		}

		usleep(DELTAT);

		//basic code to handle ship movement
		if (gfx_event_waiting()) {
			c = gfx_wait();

			switch (c) {
				case 'R' : 	// move space ship forward
					//no change in direction
					if (shipSpeed <= 8)
						shipSpeed++;
					break;
				case 'Q' : 	// rotate space ship left
					// adjust dir to reflect leftward rotation
					// x and y position doesn't change
					dir += (M_PI) / 6;
					break;
				case 'S' : 	// rotate space ship right
					// adjust dir to reflect rightward rotation
					// x and y position doesn't change
					dir -= (M_PI) / 6;
					break;
				case 'T' :	// brake space ship
					if (shipSpeed > 0) 
						shipSpeed--;
					break;
				case 'q' :
					i = 0;
					break;
			}
		}
		
		//adjust ships position
		shipXPos += (int) shipSpeed * cos(dir);
		shipYPos -= (int) shipSpeed * sin(dir);		

		gfx_flush();
	}


	//}
}

// This function initializes the window for gameplay
void setupWindow(int xSize, int ySize) {
	gfx_open(xSize, ySize, "Dodge the Asteroid");
	gfx_clear();
	gfx_color(255, 255, 255); //Set the background to black
}

// This function draws the ship at the specified location
void drawShip(int xPos, int yPos, double dir) {
	double n = (2 * M_PI) / 3;
	int i;
	for (i = 0; i < 3; i++) {
		if (i == 1) //make the rear of the ship red
			gfx_color(255, 0, 0);
		else 
			gfx_color(0, 255, 0); //the rest is green
		gfx_line(xPos+8*cos((i*n)+dir), yPos-8*sin((i*n)+dir), 
				xPos+8*cos(((i+1)*n)+dir), yPos-8*sin(((i+1)*n)+dir));
	}
}

// This function ends a game and determines if the user wants another one
int endGame(int *game, int *gL, int *n) {
	gfx_clear();
	gfx_text(150, 100, "You died.");
	gfx_text(150, 115, "Press space to play again");
	gfx_text(150, 130, "Press q to quit.");
	int k = 1;
	while (k) {
	char c = gfx_wait();
		switch (c) {
			case 'q' : //end game, close window
				*game = 0;
				*gL = 0; //set main() while loop to stop
				k = 0;
				break;
			case ' ' : //start new game
				*n = 3; //set numLives to 3
				k = 0;
				break;
			default :
				break;
		}
	}
	return *gL;
}

// This function simply changes the lives displayed
void adjustLivesTicker(int n) {
	gfx_text(290, 340, "Lives: ");
	switch (n) {
		case 3 :
			gfx_text(340, 340, "3");
			break;
		case 2 :
			gfx_text(340, 340, "2");
			break;
		case 1 :
			gfx_text(340, 340, "1");
			break;
	}
}

// This function handles the ship when it leaves the screen
void wrapShipAround(int *x, int *y) {
	if (*x >= 350)
		*x = 1;
	if (*x <= 0)
		*x = 349;
	if (*y >= 350)
		*y = 1;
	if (*y <= 0)
		*y = 349;
}

// This function varies the radius
int varyRadius(int c, int j, int r) {
	if (j % 10 == 0) {
		if (r > 24)
			c = -1;
		else if (r < 11)
			c = 1;
		r += c;
	}
	return r;
}

// This function resets the main gameplay variables, using pointers
void newLife(int *cX, int *cY, int *cXD, int *cYD, int *sS, int *x, int *y) {
	*cX = (rand() % 350);
	*cY = (rand() % 350);
	if (abs(*cX - 175) < 15 && abs(*cY - 175) < 15)
		*cX = 35;
	*cXD = (rand() % 12) - 6;
	*cYD = (rand() % 12) - 6;
	*sS = 0;
	*x = 175;
	*y = 175;
}
