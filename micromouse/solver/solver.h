#ifndef SOLVER_H
#define SOLVER_H

#include "mouse.h"
#include <stdbool.h>

typedef struct {
	int x;
	int y;
} Coord;

typedef struct {
	Coord queue[100];
	int front;
	int back;
} Queue;

typedef struct {
	int mDist[16][16];
	bool knownVWalls[16][17];
	bool knownHWalls[17][16];
} Tracking;

Action solver(Mouse *mouse);
Action leftWallFollower(Mouse *mouse);
Action floodFill(Mouse *mouse);

void push(Queue* q, Coord c);
void pop(Queue* q);
Coord front(Queue* q);
bool empty(Queue* q);

void initWallsAndQueue();

#endif
