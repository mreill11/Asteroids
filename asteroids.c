/* asteroids.c
 * Matt Reilly and Chris Beaufils
 * This is our final project, a recreation of the classic game Asteroids.
 * We are using the C language and the gfx library for graphics.
 */

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "gfx4.h"

void setupWindow(int xSize, int ySize);
void drawShip(int xPos, int yPos, int dir);

int main() {
	int xSize = 350, ySize = 350; //The window size we determined was best
	setupWindow(xSize, ySize);
	drawShip(xSize / 2, ySize / 2, 6);
	gfx_wait();
}

// This function initializes the window for gameplay
void setupWindow(int xSize, int ySize) {
	gfx_open(xSize, ySize, "Asteroids");
	gfx_clear();
	gfx_color(255, 255, 255); //Set the background to black
}

// This function draws the ship at the specified location
void drawShip(int xPos, int yPos, int dir) {
	double n = (2 * M_PI) / 3;
	dir = ((2 * M_PI) / 12) * dir;
	int i;
	for (i = 0; i < 3; i++)
		gfx_line(xPos+10*cos((i+dir)*n), yPos+10*sin((i+dir)*n), 
				xPos+10*cos((i+1+dir)*n), yPos+10*sin((i+1+dir)*n));
}
