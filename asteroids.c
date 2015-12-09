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
void drawShip(int xPos, int yPos, double angle);
void drawAsteroid(double direction, int xLoc, int yLoc, int speed, int radius);
void handleAsteroid();

struct Asteroids {
	int xLoc;
	int yLoc;
	int speed;
	int radius;
	double direction;
};

int main() {
	int xSize = 350, ySize = 350, i = 1, dir = (M_PI) / 2; //The window size we determined was best
	char c;
	double angle = .75;
	int shipXPos = 175, shipYPos = 175;

	struct Asteroids info[4];
	int count;
	for (count = 0; count < 4; count++)
		handleAsteroid(info[count]);
	setupWindow(xSize, ySize);
	drawShip(xSize / 2, ySize / 2, dir);
	while (i) {
		c = gfx_wait();

		switch (c) {
			case 'w' : 	// move space ship forward
				gfx_clear();
				//no change in direction
				shipXPos += (int) 2 * cos(dir);
				shipYPos -= (int) 2 * sin(dir);
				drawShip(shipXPos, shipYPos, angle);
				break;
			case 'a' : 	// rotate space ship left
				gfx_clear();
				// adjust dir to reflect leftward rotation
				// x and y position doesn't change
				dir += (M_PI) / 6;
				angle += .25;
				drawShip(shipXPos, shipYPos, angle);
				break;
			case 'd' : 	// rotate space ship right
				gfx_clear();
				// adjust dir to reflect rightward rotation
				// x and y position doesn't change
				dir -= (M_PI) / 6;
				angle -= .25;
				drawShip(shipXPos, shipYPos, angle);
				break;
			case 's' :	// brake space ship
				gfx_clear();
				break;
			case 'q' :
				i = 0;
				break;
		}
	/*	
		if (shipXPos >= 350)
			shipXPos = 1;
		if (shipXPos <= 0)
			shipXPos = 349;
		if (shipYPos >= 350);
			shipYPos = 1;
		if (shipYPos <= 0);
			shipYPos = 349;
	*/
		gfx_flush();
	}

}

// This function initializes the window for gameplay
void setupWindow(int xSize, int ySize) {
	gfx_open(xSize, ySize, "Asteroids");
	gfx_clear();
	gfx_color(255, 255, 255); //Set the background to black
}

// This function draws the ship at the specified location
void drawShip(int xPos, int yPos, double angle) {
	double n = (2 * M_PI) / 3;
	int i;
	for (i = 0; i < 3; i++)
		gfx_line(xPos+8*cos((i+angle)*n), yPos-8*sin((i+angle)*n), 
				xPos+8*cos((i+1+angle)*n), yPos-8*sin((i+1+angle)*n));
}

// This function draws the asteroids and assigns them an initial velocity
void drawAsteroid(double direction,int xLoc, int yLoc, int speed, int radius) {
	gfx_circle(xLoc, yLoc, radius);
}
