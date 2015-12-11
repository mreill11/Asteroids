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

struct Asteroids {
	int xLoc;
	int yLoc;
	int speed;
	int radius;
};

void setupWindow(int xSize, int ySize);
void drawShip(int xPos, int yPos, double dir);
void drawAsteroid(int xLoc, int yLoc, int radius);
//void handleAsteroid(struct Asteroids);
void animateBullet(int n, double bullets[40][4]);

int main() {
	int xSize = 350, ySize = 350, i = 1, shipSpeed = 3;
	double dir = (M_PI) / 2; //The window size we determined was best
	char c;
	double angle = .75;
	int shipXPos = 175, shipYPos = 175;
	double bullets[40][4];
	int numBulletsActive = 0;

	//	struct Asteroids bigAsteroids[4];
	
	//	handleAsteroid(bigAsteroids[0]);
	
	setupWindow(xSize, ySize);
	drawShip(xSize / 2, ySize / 2, dir);
	while (i) {
		gfx_clear();
		drawShip(shipXPos, shipYPos, dir);
		animateBullet(numBulletsActive, bullets);
		
		if (shipXPos >= 350)
			shipXPos = 1;
		if (shipXPos <= 0)
			shipXPos = 349;
		if (shipYPos >= 350)
			shipYPos = 1;
		if (shipYPos <= 0)
			shipYPos = 349;
		//printf("%c\n", c);
		
		usleep(DELTAT);
		if (gfx_event_waiting()) {
			c = gfx_wait();

			switch (c) {
				case ' ' :
					bullets[numBulletsActive][0] = (double) shipXPos;
					bullets[numBulletsActive][1] = (double) shipYPos;
					bullets[numBulletsActive][2] = dir;
					bullets[numBulletsActive][3] = 1;
					//animateBullet(numBulletsActive, bullets);
					numBulletsActive++;
					break;
				case 'R' : 	// move space ship forward
					//gfx_clear();
					//no change in direction
					if (shipSpeed <= 8)
						shipSpeed++;
					shipXPos += (int) shipSpeed * cos(dir);
					shipYPos -= (int) shipSpeed * sin(dir);
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'Q' : 	// rotate space ship left
					//gfx_clear();
					// adjust dir to reflect leftward rotation
					// x and y position doesn't change
					dir += (M_PI) / 6;
					angle += .25;
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'S' : 	// rotate space ship right
					//gfx_clear();
					// adjust dir to reflect rightward rotation
					// x and y position doesn't change
					dir -= (M_PI) / 6;
					angle -= .25;
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'T' :	// brake space ship
					//	gfx_clear();
					shipSpeed--;
					if (shipSpeed > 0) {
						shipXPos += (int) shipSpeed * cos(dir);
						shipYPos -= (int) shipSpeed * sin(dir);
					} else {
						shipSpeed = 3;
					}
					break;
				case 'q' :
					i = 0;
					break;
			}
		}
		
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
/*
void handleAsteroid(struct Asteroids bigAsteroids) {
	int i = 0;
	//for (i = 0; i < 4; i++) {
		bigAsteroids.xLoc = (rand() % 350);
		bigAsteroids.yLoc = 0;
		bigAsteroids.speed = (rand() % 4);
		bigAsteroids.radius = (rand() % 20) + 10;
		drawAsteroid(bigAsteroids.xLoc, bigAsteroids.yLoc, bigAsteroids.radius);
	//}
}
*/
void animateBullet(int n, double bullets[40][4]) {
	int x = (int) bullets[n][0];
	int y = (int) bullets[n][1];
	double dir = bullets[n][2];
	int active = (int) bullets[n][3];
	if (active) {
		gfx_line(x - 8 * cos(dir), y - 8 * sin(dir), x - 12 * cos(dir), y - 12 * sin(dir));
		bullets[n][0] += 8 * cos(dir);
		bullets[n][1] += 8 * sin(dir);
	}
}
