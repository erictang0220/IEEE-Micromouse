#include "solver.h"
#include "mouse.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

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


Queue q;
Tracking t;
Action floodFill(Mouse *mouse)
{
	// starts off at (0, 0)?
	Coord tmp;
	tmp.x = 0;
	tmp.y = 0;
	push(&q, tmp);
	
	
	// while the queue is not empty
	while(!empty(&q)) {
		
		Coord c = front(&q);
		pop(&q);
		
		// detect walls
		Heading curH = getHeading(mouse);
		switch (curH) {
			case NORTH:
				if(getFrontReading(mouse)) {
					t.knownHWalls[c.x][c.y] = true;
				}
				if(getLeftReading(mouse)) {
					t.knownVWalls[c.x][c.y] = true;
				}
				if(getRightReading(mouse)) {
					t.knownVWalls[c.x][c.y+1] = true;
				}
				break;
			case SOUTH:
				if(getFrontReading(mouse)) {
					t.knownHWalls[c.x+1][c.y] = true;
				}
				if(getLeftReading(mouse)) {
					t.knownVWalls[c.x][c.y+1] = true;
				}
				if(getRightReading(mouse)) {
					t.knownVWalls[c.x][c.y] = true;
				}
				break;
			case EAST:
				if(getFrontReading(mouse)) {
					t.knownVWalls[c.x][c.y+1] = true;
				}
				if(getLeftReading(mouse)) {
					t.knownHWalls[c.x][c.y] = true;
				}
				if(getRightReading(mouse)) {
					t.knownHWalls[c.x+1][c.y] = true;
				}
				break;
			case WEST:
				if(getFrontReading(mouse)) {
					t.knownVWalls[c.x][c.y] = true;
				}
				if(getLeftReading(mouse)) {
					t.knownHWalls[c.x+1][c.y] = true;
				}
				if(getRightReading(mouse)) {
					t.knownHWalls[c.x][c.y] = true;
				}
				break;
		}
		
		
		int min = INT_MAX;
		
		// if neighbors not wall and less than min, change min
		if(t.mDist[c.x][c.y-1] < min && !t.knownHWalls[c.x][c.y-1]) {
			min = t.mDist[c.x][c.y-1];
		}
		if(t.mDist[c.x-1][c.y] < min && !t.knownVWalls[c.x-1][c.y]) {
			min = t.mDist[c.x-1][c.y];
		}
		if(t.mDist[c.x+1][c.y] < min && !t.knownVWalls[c.x+1][c.y]) {
			min = t.mDist[c.x+1][c.y];
		}
		if(t.mDist[c.x][c.y+1] < min && !t.knownHWalls[c.x][c.y+1]) {
			min = t.mDist[c.x][c.y+1];
		}
		
		// if current cell <= min of neighbors
		if(t.mDist[c.x][c.y] <= min && min != INT_MAX) {
			t.mDist[c.x][c.y] = ++min;
			
			//while not wall
			if(!t.knownHWalls[c.x][c.y+1]) {
				Coord tmp;
				tmp.x = c.x;
				tmp.y = c.y+1;
				push(&q, tmp);
			}
			if(!t.knownVWalls[c.x+1][c.y]) {
				Coord tmp;
				tmp.x = c.x+1;
				tmp.y = c.y;
				push(&q, tmp);
			}
			if(!t.knownVWalls[c.x-1][c.y]) {
				Coord tmp;
				tmp.x = c.x-1;
				tmp.y = c.y;
				push(&q, tmp);
			}
			if(!t.knownHWalls[c.x][c.y-1]) {
				Coord tmp;
				tmp.x = c.x;
				tmp.y = c.y-1;
				push(&q, tmp);
			}
			return IDLE;
		}
		// else move to the cell with the min value depending on the current heading
		else {
			if(curH == NORTH) {
				if(t.mDist[c.x][c.y-1] == min) {
					return LEFT;
				}
				
				if(t.mDist[c.x][c.y+1] == min) {
					return RIGHT;
				}
				
				if(t.mDist[c.x-1][c.y] == min) {
					return FORWARD;
				}
			}
			if(curH == SOUTH) {
				if(t.mDist[c.x][c.y+1] == min) {
					return LEFT;
				}
				
				if(t.mDist[c.x][c.y-1] == min) {
					return RIGHT;
				}
				
				if(t.mDist[c.x+1][c.y] == min) {
					return FORWARD;
				}
			}
			if(curH == EAST) {
				if(t.mDist[c.x-1][c.y] == min) {
					return LEFT;
				}
				
				if(t.mDist[c.x+1][c.y] == min) {
					return RIGHT;
				}
				
				if(t.mDist[c.x][c.y+1] == min) {
					return FORWARD;
				}
			}
			if(curH == WEST) {
				if(t.mDist[c.x+1][c.y] == min) {
					return LEFT;
				}
				
				if(t.mDist[c.x-1][c.y] == min) {
					return RIGHT;
				}
				
				if(t.mDist[c.x][c.y-1] == min) {
					return FORWARD;
				}
			}
			
			// meet dead end??
			return IDLE;
		}
	}
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

void initWallsAndQueue() {
	
	// the center cell is mDist[7][7]??
	// initializing Manhattan distances
	for(int i=0; i < 16; i++) {
		for(int j=0; j < i; j++) {
			t.mDist[i][j] = abs(i-7) + abs(j-7);
			t.mDist[j][i] = abs(i-7) + abs(j-7);
		}
	}
	
	// initializing queue
	q.front = 0;
	q.back = 0;
}
