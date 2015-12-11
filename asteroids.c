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
	int xSize = 350, ySize = 350, i = 1, shipSpeed = 0;
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

	//	struct Asteroids bigAsteroids[4];
	//	handleAsteroid(bigAsteroids[0]);
	
	setupWindow(xSize, ySize);
	drawShip(xSize / 2, ySize / 2, dir);
	gfx_circle(circleX, circleY, radius);
	while (i) {
		gfx_clear();
		gfx_text(290, 340, "Lives: ");
		switch (numLives) {
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
		circleX += circleXDirection;
		circleY += circleYDirection;
		gfx_circle(circleX, circleY, radius);
		if (j % 10 == 0) {
			if (radius > 24)
				count = -1;
			else if (radius < 11)
				count = 1;
			radius += count;
		}
		j++;
		if (circleX - radius <= 0 || circleX + radius >= xSize)
			circleXDirection = -circleXDirection;
		if (circleY - radius <= 0 || circleY + radius >= ySize)
			circleYDirection = -circleYDirection;
		drawShip(shipXPos, shipYPos, dir);
		//animateBullet(numBulletsActive, bullets);
		
		if (shipXPos >= 350)
			shipXPos = 1;
		if (shipXPos <= 0)
			shipXPos = 349;
		if (shipYPos >= 350)
			shipYPos = 1;
		if (shipYPos <= 0)
			shipYPos = 349;
		//printf("%c\n", c);
		
		if (shipXPos > (circleX - radius) && shipXPos < (circleX + radius) && shipYPos > (circleY - radius) && shipYPos < (circleY + radius)) {
			usleep(1000);
			numLives--;
			char c = gfx_wait();
			if (c == ' ')
				continue;
		} 

		if (numLives == 0)
			i = 0;

		usleep(DELTAT);
		if (gfx_event_waiting()) {
			c = gfx_wait();

			switch (c) {
				case ' ' :
					bullets[numBulletsActive][0] = (double) shipXPos;
					bullets[numBulletsActive][1] = (double) shipYPos;
					bullets[numBulletsActive][2] = dir;
					bullets[numBulletsActive][3] = 1;
					animateBullet(numBulletsActive, bullets);
					numBulletsActive++;
					break;
				case 'R' : 	// move space ship forward
					//gfx_clear();
					//no change in direction
					if (shipSpeed <= 8)
						shipSpeed++;
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'Q' : 	// rotate space ship left
					//gfx_clear();
					// adjust dir to reflect leftward rotation
					// x and y position doesn't change
					dir += (M_PI) / 6;
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'S' : 	// rotate space ship right
					//gfx_clear();
					// adjust dir to reflect rightward rotation
					// x and y position doesn't change
					dir -= (M_PI) / 6;
					//drawShip(shipXPos, shipYPos, dir);
					break;
				case 'T' :	// brake space ship
					//	gfx_clear();
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
	int i = 0;
	while (i <= n) {
		int x = (int) bullets[n][0];
		int y = (int) bullets[n][1];
		double dir = bullets[n][2];
		int active = (int) bullets[n][3];
		if (active) {
			gfx_line(x, y, x - 12 * cos(dir), y - 12 * sin(dir));
			bullets[n][0] += 8 * cos(dir);
			bullets[n][1] += 8 * sin(dir);
		}
		if (bullets[n][0] > 350 || bullets[n][0] < 0)
			bullets[n][3] = 0;
		if (bullets[n][1] > 350 || bullets[n][1] < 0)
			bullets[n][3] = 0;
	}
}
