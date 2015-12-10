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

typedef struct Asteroids {
	int xLoc;
	int yLoc;
	int speed;
	int radius;
} Asteroids;

void setupWindow(int xSize, int ySize);
void drawShip(int xPos, int yPos, double dir);
void drawAsteroid(int xLoc, int yLoc, int radius);
void handleAsteroid(struct Asteroids *bigAsteroids);

int main() {
	int xSize = 350, ySize = 350, i = 1;
	double dir = (M_PI) / 2; //The window size we determined was best
	char c;
	double angle = .75;
	int shipXPos = 175, shipYPos = 175;

	Asteroids bigAsteroids[4];
	
	//int count;
	//for (count = 0; count < 4; count++)
	handleAsteroid(bigAsteroids);
	
	setupWindow(xSize, ySize);
	drawShip(xSize / 2, ySize / 2, dir);
	while (i) {
		c = gfx_wait();

		switch (c) {
			case 'w' : 	// move space ship forward
				gfx_clear();
				//no change in direction
				shipXPos += (int) 5 * cos(dir);
				shipYPos -= (int) 5 * sin(dir);
				drawShip(shipXPos, shipYPos, dir);
				break;
			case 'a' : 	// rotate space ship left
				gfx_clear();
				// adjust dir to reflect leftward rotation
				// x and y position doesn't change
				dir += (M_PI) / 6;
				angle += .25;
				drawShip(shipXPos, shipYPos, dir);
				break;
			case 'd' : 	// rotate space ship right
				gfx_clear();
				// adjust dir to reflect rightward rotation
				// x and y position doesn't change
				dir -= (M_PI) / 6;
				angle -= .25;
				drawShip(shipXPos, shipYPos, dir);
				break;
			case 's' :	// brake space ship
				//	gfx_clear();
				break;
			case 'q' :
				i = 0;
				break;
		}
		
		if (shipXPos >= 350)
			shipXPos = 1;
		if (shipXPos <= 0)
			shipXPos = 349;
		if (shipYPos >= 350)
			shipYPos = 1;
		if (shipYPos <= 0)
			shipYPos = 349;
	
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
void drawShip(int xPos, int yPos, double dir) {
	double n = (2 * M_PI) / 3;
	int i;
	for (i = 0; i < 3; i++)
		gfx_line(xPos+8*cos((i*n)+dir), yPos-8*sin((i*n)+dir), 
				xPos+8*cos(((i+1)*n)+dir), yPos-8*sin(((i+1)*n)+dir));
}

// This function draws the asteroids and assigns them an initial velocity
void drawAsteroid(int xLoc, int yLoc, int radius) {
	gfx_circle(xLoc, yLoc, radius);
}

void handleAsteroid(Asteroids *bigAsteroids) {
	int i;
	for (i = 0; i < 4; i++) {
		bigAsteroids[i].xLoc = (rand() % 350);
		bigAsteroids[i].yLoc = 0;
		bigAsteroids[i].speed = (rand() % 4);
		bigAsteroids[i].radius = (rand() % 20) + 10;
		drawAsteroid(bigAsteroids[i].xLoc, bigAsteroids[i].yLoc, bigAsteroids[i].radius);
	}
}
