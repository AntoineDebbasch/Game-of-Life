/*
 * lab1a.c
 *
 *  Created on:
 *      Author: Antoine DEBBASCH
 */

/* include helper functions for game */
#include "lifegame.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/* add whatever other includes here */

/* number of generations to evolve the world */
#define NUM_GENERATIONS 50

/* functions to implement */

/* this function should set the state of all
   the cells in the next generation and call
   finalize_evolution() to update the current
   state of the world to the next generation */
void next_generation(void);

/* this function should return the state of the cell
   at (x,y) in the next generation, according to the
   rules of Conway's Game of Life (see handout) */
int get_next_state(int x, int y);

/* this function should calculate the number of alive
   neighbors of the cell at (x,y) */
int num_neighbors(int x, int y);

int main(void)
{

	/* TODO: initialize the world */
	
	initialize_world();
	print_world();

	for (int n = 0; n < NUM_GENERATIONS; n++)
	{

		//system("tput clear");

		printf("Generation n : %d\n", n + 1);
		next_generation();
		print_world();
		usleep(300000);

		if (is_World_Empty())
			n = NUM_GENERATIONS;
	}

	/* TODO: output final world state */

	puts("Hello World !!");

	return 0;
}

void next_generation(void)
{
	/* TODO: for every cell, set the state in the next
	   generation according to the Game of Life rules

	   Hint: use get_next_state(x,y) and set_next_state(x,y) */

	for (int x = 0; x < get_world_width(); x++)
		for (int y = 0; y < get_world_height(); y++)
			set_cell_state(x, y, get_next_state(x, y));

	finalize_evolution(); /* called at end to finalize */
}

int get_next_state(int x, int y)
{
	/* TODO: for the specified cell, compute the state in
	   the next generation using the rules

	   Use num_neighbors(x,y) to compute the number of live
	   neighbors */

	int neighbors = num_neighbors(x, y);
	int state = get_cell_state(x, y);

	if (state == ALIVE)
	{
		if (neighbors > 3 || neighbors < 2)
			return DEAD;
		else
			return ALIVE;
	}

	else if (state == DEAD)
		if (neighbors == 3)
			return ALIVE;

	return 0;
}

int num_neighbors(int x, int y)
{
	int cpt = 0;

	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
			if ((dx != 0 || dy != 0) && get_cell_state(x + dx, y + dy) == ALIVE)
				cpt++;

	//   x-1 | x   | x+1
	//   y-1 | y-1 | y-1
	//   ----|-----|-----
	//   x-1 | x   | x+1
	//   y   | y   | y
	//   ----|-----|-----
	//   x-1 | x   | x+1
	//   y+1 | y+1 | y+1

	return cpt;
}


