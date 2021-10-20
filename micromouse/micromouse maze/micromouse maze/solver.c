#include "solver.h"
#include "mouse.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// This function redirects function calls from mouse.c to the desired maze solving algorithm
Action solver(Mouse *mouse)
{
	// This can be changed to call other maze solving algorithms
	return floodFill(mouse);
}

// Simple algorithm; mouse goes straight until encountering a wall, then preferentially turns left
Action obstacleAvoider(Mouse *mouse)
{
	if(getFrontReading(mouse) == 0) return FORWARD;
	else if(getLeftReading(mouse) == 0) return LEFT;
	else if(getRightReading(mouse) == 0) return RIGHT;
	else return LEFT;
}

// Left wall following algorithm
int turnedLeft = 0;
Action leftWallFollower(Mouse *mouse)
{
	if(turnedLeft)
		{
		turnedLeft = 0;
		return FORWARD;
		}
	else if(getLeftReading(mouse) == 0)
		{
		turnedLeft = 1;
		return LEFT;
		}
	else if(getFrontReading(mouse) == 0) return FORWARD;
	else return RIGHT;
}

Tracking t;
Queue q;
int hasNotInit = 1;
Action floodFill(Mouse *mouse)
{
	// initialize walls, manhattan distance, and queue the first time floodfill is called
	if(hasNotInit) {
		initWallsAndQueue(mouse);
		hasNotInit = 0;
	}
	
	// currrent location of the mouse
	int x = mouse->maze->size - mouse->y - 1;
	int y = mouse->x;
	
	// if reached the center, don't move
	if(t.mDist[x][y] == 0) return IDLE;
	
	Heading curH = getHeading(mouse);
	
	// updating the walls
	switch (curH) {
		case NORTH:
			if(getFrontReading(mouse)) {
				t.knownHWalls[x][y] = true;
			}
			if(getLeftReading(mouse)) {
				t.knownVWalls[x][y] = true;
			}
			if(getRightReading(mouse)) {
				t.knownVWalls[x][y+1] = true;
			}
			break;
		case SOUTH:
			if(getFrontReading(mouse)) {
				t.knownHWalls[x+1][y] = true;
			}
			if(getLeftReading(mouse)) {
				t.knownVWalls[x][y+1] = true;
			}
			if(getRightReading(mouse)) {
				t.knownVWalls[x][y] = true;
			}
			break;
		case EAST:
			if(getFrontReading(mouse)) {
				t.knownVWalls[x][y+1] = true;
			}
			if(getLeftReading(mouse)) {
				t.knownHWalls[x][y] = true;
			}
			if(getRightReading(mouse)) {
				t.knownHWalls[x+1][y] = true;
			}
			break;
		case WEST:
			if(getFrontReading(mouse)) {
				t.knownVWalls[x][y] = true;
			}
			if(getLeftReading(mouse)) {
				t.knownHWalls[x+1][y] = true;
			}
			if(getRightReading(mouse)) {
				t.knownHWalls[x][y] = true;
			}
			break;
	}
	
	// TODO: if possible move toward the lower value
	printf("\nhorizontal walls\n");
	
	for(int i=0; i < mouse->maze->size+1; i++) {
		for(int j=0; j < mouse->maze->size; j++) {
			printf("%d ",t.knownHWalls[i][j]);
		}
		printf("\n");
	}
	
	printf("\nvertical walls\n");
	
	for(int i=0; i < mouse->maze->size; i++) {
		for(int j=0; j < mouse->maze->size+1; j++) {
			printf("%d ",t.knownVWalls[i][j]);
		}
		printf("\n");
	}
	
	
	int curMDist = t.mDist[x][y];
	// if there's a neighboring cell that has a lower value, go in that direction
	switch(curH) {
		case NORTH:
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return FORWARD;
			}
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return RIGHT;
			}
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return LEFT;
			}
			break;
		case SOUTH:
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return FORWARD;
			}
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return LEFT;
			}
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return RIGHT;
			}
			break;
		case EAST:
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return FORWARD;
			}
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return LEFT;
			}
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return RIGHT;
			}
			break;
		case WEST:
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return FORWARD;
			}
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return RIGHT;
			}
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return LEFT;
			}
			break;
			
	}
	
	// push the current location onto the queue
	Coord tmp;
	tmp.x = mouse->maze->size - mouse->y;
	tmp.y = mouse->x;
	push(&q, tmp);
	
	// update Manhattan distance in the loop
	while(!empty(&q)) {
		
		Coord c = front(&q);
		pop(&q);
		int min = INT_MIN;
		
		// get the mininal neighbor
		// north cell
		if(t.mDist[tmp.x-1][tmp.y] < min && !t.knownHWalls[tmp.x-1][tmp.y]) {
			min = t.mDist[tmp.x-1][tmp.y];
		}
		// south cell
		if(t.mDist[tmp.x+1][tmp.y] < min && !t.knownHWalls[tmp.x+1][tmp.y]) {
			min = t.mDist[tmp.x+1][tmp.y];
		}
		// east cell
		if(t.mDist[tmp.x][tmp.y+1] < min && !t.knownVWalls[tmp.x][tmp.y+1]) {
			min = t.mDist[tmp.x][tmp.y+1];
		}
		// west cell
		if(t.mDist[tmp.x][tmp.y-1] < min && !t.knownVWalls[tmp.x][tmp.y-1]) {
			min = t.mDist[tmp.x][tmp.y-1];
		}
		
		// if current cell <= min of neighbors
		if(t.mDist[c.x][c.y] <= min) {
			
			// increment the current cell
			t.mDist[c.x][c.y] = ++min;
			
			//while not wall
			if(!t.knownHWalls[c.x][c.y+1]) {
				Coord tmp = {c.x, c.y+1};
				push(&q, tmp);
			}
			if(!t.knownVWalls[c.x+1][c.y]) {
				Coord tmp = {c.x+1, c.y};
				push(&q, tmp);
			}
			if(!t.knownVWalls[c.x-1][c.y]) {
				Coord tmp = {c.x-1, c.y};
				push(&q, tmp);
			}
			if(!t.knownHWalls[c.x][c.y-1]) {
				Coord tmp = {c.x, c.y-1};
				push(&q, tmp);
			}
		}
	}
	
	// get the updated manhattan distance
	curMDist = t.mDist[x][y];
	
	// Go to the neighboring cell with a lower value
	switch(curH) {
		case NORTH:
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return FORWARD;
			}
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return RIGHT;
			}
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return LEFT;
			}
			break;
		case SOUTH:
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return FORWARD;
			}
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return LEFT;
			}
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return RIGHT;
			}
			break;
		case EAST:
			if(t.mDist[x][y+1] < curMDist && !t.knownVWalls[x][y+1]) {
				return FORWARD;
			}
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return LEFT;
			}
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return RIGHT;
			}
			break;
		case WEST:
			if(t.mDist[x][y-1] < curMDist && !t.knownVWalls[x][y-1]) {
				return FORWARD;
			}
			if(t.mDist[x-1][y] < curMDist && !t.knownHWalls[x-1][y]) {
				return RIGHT;
			}
			if(t.mDist[x+1][y] < curMDist && !t.knownHWalls[x+1][y]) {
				return LEFT;
			}
			break;
			
	}
	
	// only reached here when it hits a dead end
	return LEFT;
}

void push(Queue* q, Coord c) {
	q->queue[q->back++] = c;
	if(q->back >= 100) q->back = 0;
}

void pop(Queue* q) {
	// if queue is empty, do nothing
	if(q->front == q->back)
		return;
	
	// erase the first element
	q->front++;
	if(q->front >= 100) q->front = 0;
}

Coord front(Queue* q) {
	// have to be sure it's not empty
	return q->queue[q->front];
}

bool empty(Queue* q) {
	return (q->front == q->back);
}

void initWallsAndQueue(Mouse *mouse) {
	
	// the center cell is mDist[7][7]??
	// initializing Manhattan distances
	int mSize = mouse->maze->size;
	int halfSize = mSize / 2 - 1;
	for(int i=0; i < mSize; i++) {
		for(int j=0; j <= i; j++) {
			if(mSize % 2 == 1 || (i <= halfSize && j <= halfSize)) {
				t.mDist[i][j] = abs(i-halfSize) + abs(j-halfSize);
				t.mDist[j][i] = abs(i-halfSize) + abs(j-halfSize);
			}
			else {
				if(j > halfSize) {
					t.mDist[i][j] = t.mDist[i][mSize-j-1];
					t.mDist[j][i] = t.mDist[i][mSize-j-1];
				}
				if(i > halfSize) {
					t.mDist[i][j] = t.mDist[mSize-i-1][j];
					t.mDist[j][i] = t.mDist[mSize-i-1][j];
				}
			}
		}
	}
	
	// Tyler's implementation
	//	int mL = 16;
	//	// Initializes default manhattan distance array given maze dimensions
	//	for (int i = 0; i < mL; i++)
	//	{
	//	for (int j = 0; j < mL; j++)
	//		{
	//		if (mL % 2 == 0)
	//			{
	//			t.mDist[i][j] = abs(mL / 2 - i - ((i < mL / 2) ? 1 : 0)) + abs(mL / 2 - j - ((j < mL / 2) ? 1 : 0));
	//			}
	//		else
	//			{
	//			t.mDist[i][j] = abs(floor(mL / 2) - i) + abs(floor(mL / 2) - j);
	//			}
	//		}
	//	}
	
	//	for(int i=0; i < mSize; i++) {
	//		for(int j=0; j < mSize; j++) {
	//			printf("%02d ", t.mDist[i][j]);
	//		}
	//		printf("\n");
	//	}
	
	// all walls implicity initialized to false
	
	// initializing queue
	q.front = 0;
	q.back = 0;
}
