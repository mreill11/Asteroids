/* asteroids.c
 * Matt Reilly and Chris Beaufils
 * This is our final project, a recreation of the classic game Asteroids.
 * We are using the C language and the gfx library for graphics.
 */

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "gfx4.h"
#define DELTAT 10000

void setupWindow(int xSize, int ySize);
void drawShip(int xPos, int yPos, double dir);
void endGame(int *game, int *gL);
void newGame();
//void newLife();
void adjustLivesTicker(int n);
void wrapShipAround(int *x, int *y);
int varyRadius(int c, int j, int r);

int main() {
	int gameLive = 1;
	int *gL = &gameLive;
	setupWindow(350, 350);
	while (gameLive) {
	int xSize = 350, ySize = 350, i = 1, shipSpeed = 0;
	int *game = &i;
	int circleX = (rand() % 350);
	int circleY = (rand() % 350);
	int circleXDirection = (rand() % 12) - 6, circleYDirection = (rand() % 12) - 6;
	int radius = 12;
	int count = 1, j = 0, numLives = 3;

	if (abs(circleX - 175) < 15 && abs(circleY - 175) < 15)
		circleX = 35;

	double dir = (M_PI) / 2;
	char c;
	double angle = .75;
	int shipXPos = 175, shipYPos = 175;
	double bullets[40][4];
	int numBulletsActive = 0;

	int *x = &shipXPos, *y = &shipYPos;
	
	drawShip(xSize / 2, ySize / 2, dir);
	gfx_circle(circleX, circleY, radius);

	while (i) {
		gfx_clear();
		
		adjustLivesTicker(numLives);
		
		circleX += circleXDirection;
		circleY += circleYDirection;

		gfx_circle(circleX, circleY, radius);
		
		radius = varyRadius(count, j, radius);
		j++;
		
		if (circleX - radius <= 0 || circleX + radius >= xSize)
			circleXDirection = -circleXDirection;
		if (circleY - radius <= 0 || circleY + radius >= ySize)
			circleYDirection = -circleYDirection;
		
		drawShip(shipXPos, shipYPos, dir);
		
		wrapShipAround(x, y);
		
		if (shipXPos > (circleX - radius) && shipXPos < (circleX + radius) && shipYPos > (circleY - radius) && shipYPos < (circleY + radius)) {
			usleep(1000);
			numLives--;
			if (numLives == 0)
				endGame(game, gL);
			char c = gfx_wait();
			gfx_text(150, 30, "Press space to continue");
			//if (c == ' ')
				//newLife();
		}

		usleep(DELTAT);
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
		
		shipXPos += (int) shipSpeed * cos(dir);
		shipYPos -= (int) shipSpeed * sin(dir);		

		gfx_flush();
	}


	}
}

// This function initializes the window for gameplay
void setupWindow(int xSize, int ySize) {
	gfx_open(xSize, ySize, "Asteroids");
	gfx_clear();
	gfx_color(255, 255, 255); //Set the background to black
}

// This function draws the ship at the specified location
void drawShip(int xPos, int yPos, double dir) {
	double n = (2 * M_PI) / 3;
	int i;
	for (i = 0; i < 3; i++) {
		if (i == 1) 
			gfx_color(255, 0, 0);
		else 
			gfx_color(0, 255, 0);
		gfx_line(xPos+8*cos((i*n)+dir), yPos-8*sin((i*n)+dir), 
				xPos+8*cos(((i+1)*n)+dir), yPos-8*sin(((i+1)*n)+dir));
	}
}


void endGame(int *game, int *gL) {
	gfx_clear();
	gfx_text(150, 100, "You died.");
	gfx_text(150, 115, "Press space to play again");
	gfx_text(150, 130, "Press q to quit.");
	char c = gfx_wait();
	switch (c) {
		case 'q' :
			*game = 0;
			*gL = 0;
			break;
		case ' ' :
			newGame();
			break;
		default :
			*game = 0;
			*gL = 0;
			break;
	}
}

void newGame() {
	
}

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
