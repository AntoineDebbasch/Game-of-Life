/*
 * lifegame.c
 *
 *  Created on: 04/10/2022
 *      Author: Antoine DEBBASCH
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lifegame.h"

/* hard-coded world size */
#define WORLDWIDTH 39
#define WORLDHEIGHT 20

/* character representations of cell states */
#define CHAR_ALIVE '*'
#define CHAR_DEAD ' '

/* current cell states of the world 
*/
static int world[WORLDWIDTH][WORLDHEIGHT];

/* next generation cell states 
*/
static int nextstates[WORLDWIDTH][WORLDHEIGHT];

/* functions to write for Part B of lab
 */
void initialize_world_from_file(const char *filename)
{
	/* DONE: read the state of the world from a file with
	   name "filename". Assume file exists, is readable, and
	   the ith character of the jth line (zero-indexed) describes
	   world[i][j] according to the characters CHAR_ALIVE and
	   CHAR_DEAD

	   Assume a line does not contain more than 256 characters
	   (including newline). If a line doesn't contain WORLDWIDTH
	   characters, remaining cells in line are presumed DEAD.
	   Similarly, if the file does not contain WORLDHEIGHT lines,
	   remaining lines are presumed dead.

	   On error, print some useful error message and call abort().

	   Also need to reset the next generation to DEAD
	 */

	// for all the cells
	for (int x = 0; x < WORLDWIDTH; x++)
		for (int y = 0; y < WORLDHEIGHT; y++)
			world[x][y] = DEAD; // define dead at the beginning

	FILE *ptr = fopen(filename, "r"); // we open the file
	char ch;
	int x = 0, y = 0;

	if (NULL == ptr)					   	// if the file can't be opned
		printf("file can't be opened \n"); 	// infrom the user
	else								 	// else
		do
		{
			ch = fgetc(ptr);						// we get all the char from the file one by one 
			if (ch == '\n' && y < WORLDHEIGHT)		// if it is the end of the line and it isn't bigger than the world
			{	
				y++;								// go to next line
				x = 0;								// return at the begining of the libe
			}

			else if (ch == CHAR_ALIVE)				// if it isn't the end of the line if it is the char alive
				world[x][y] = ALIVE;				// we set the cell alive

			else if (ch == CHAR_DEAD)				// if it is the char dead
				world[x][y] = DEAD;					// we set the cell dead

			if (x < WORLDWIDTH)						// if we are not exeeding the width of the world
				x++;								// we pass to the next cell

													// Checking if character is not EOF.
													// If it is EOF stop eading.
		} while (ch != EOF);
	fclose(ptr);									// close the file
}

/* this function save the world to a file
*/
void save_world_to_file(const char *filename)
{
	/* DONE: write the state of the world into a file with
	   name "filename". Assume the file can be created, or if
	   the file exists, overwrite the file. The ith character
	   of the jth line (zero-indexed) describes world[i][j]
	   using the characters CHAR_ALIVE and CHAR_DEAD

	   This file should be readable using the function
	   initialize_world_from_file(filename) above; we can use
	   it to resume a game later
	 */

	FILE *fic = NULL;
	fic = fopen(filename, "w");						// we open the file in a writable way
	int state;

	if (fic == NULL)								// if the file isn't open	
	{		
		printf("Erreur d’ouverture de fichier\n");	// we inform the user
		}
	else											// else
	{												// for all the cells
		for (int y = 0; y < WORLDHEIGHT; y++)
		{
			for (int x = 0; x < WORLDWIDTH; x++)
			{
				state = get_cell_state(x, y);		// we get the sate of the cell
				if (state == ALIVE)					// if the cell is alive
				{
					fprintf(fic, "*");				// write in the file that it is alive
				}

				else								// else
					fprintf(fic, " ");				// print that it is dead
			}
			fprintf(fic, "\n");
		}

		fclose(fic);								// then close the file
	}
}



/* initializes the world to a hard-coded pattern, and resets
   all the cells in the next generation to DEAD */
void initialize_world(void) // world[collone][ligne]
{
	int i, j;
													// for all the cell
	for (i = 0; i < WORLDWIDTH; i++)
		for (j = 0; j < WORLDHEIGHT; j++)
			world[i][j] = nextstates[i][j] = DEAD;	// set it to dead 

	/* pattern "glider"*/
	world[1][2] = ALIVE;							// Draw the glider patern cell by cell
	world[3][1] = ALIVE;
	world[3][2] = ALIVE;
	world[3][3] = ALIVE;
	world[2][3] = ALIVE;

	/*
	 |       *
	 |     * *
	 |      **
	*/
}

/* Tets if the world is empty or not
*/
int is_World_Empty(void) 
{												// for all the cells 
	for (int i = 0; i < WORLDWIDTH; i++)
		for (int j = 0; j < WORLDHEIGHT; j++)
			if (world[i][j] == ALIVE)			// if at least one is alive 
				return 0;						// then the world isn't empty
	return 1;									// else it is empty
}

/*Get the width of the worl
*/
int get_world_width(void) 
{
	return WORLDWIDTH;
}

/*Get the height of the world
*/
int get_world_height(void) 
{
	return WORLDHEIGHT;
}

/*Get the state of the cell
*/
int get_cell_state(int x, int y) 
{
	if (x < 0 || x >= WORLDWIDTH || y < 0 || y >= WORLDHEIGHT) 	// if the coordinate are not defined in the wordl 
		return DEAD;											// return that irt is dead
	return world[x][y];											// else return its state
}

void set_cell_state(int x, int y, int state) // Set the state of the cell
{
	if (x < 0 || x >= WORLDWIDTH || y < 0 || y >= WORLDHEIGHT)
	{
		fprintf(stderr, "Error: coordinates (%d,%d) are invalid.\n", x, y);
		abort();
	}
	nextstates[x][y] = state;
}

void finalize_evolution(void) // Finalize evolution for next generation
{
	for (int x = 0; x < WORLDWIDTH; x++)
		for (int y = 0; y < WORLDHEIGHT; y++)
		{
			world[x][y] = nextstates[x][y];
			nextstates[x][y] = DEAD;
		}
}

void output_world(void) // Function given to display the world
{
	char worldstr[2 * WORLDWIDTH + 2];
	int i, j;

	worldstr[2 * WORLDWIDTH + 1] = '\0';
	worldstr[0] = '+';
	for (i = 1; i < 2 * WORLDWIDTH; i++)
		worldstr[i] = '-';
	worldstr[2 * WORLDWIDTH] = '+';
	puts(worldstr);
	for (i = 0; i <= 2 * WORLDWIDTH; i += 2)
		worldstr[i] = '|';
	for (i = 0; i < WORLDHEIGHT; i++)
	{
		for (j = 0; j < WORLDWIDTH; j++)
			worldstr[2 * j + 1] = world[j][i] == ALIVE ? CHAR_ALIVE : CHAR_DEAD;
		puts(worldstr);
	}
	worldstr[0] = '+';
	for (i = 1; i < 2 * WORLDWIDTH; i++)
		worldstr[i] = '-';
	worldstr[2 * WORLDWIDTH] = '+';
	puts(worldstr);
}
//---------------------------------//
void print_world(void) // Function made to display the world
{

	for (int i = 0; i < WORLDWIDTH + 2; i++) printf("-");		// draw the top line of the frame

	printf("\n");

	for (int i = 0; i < WORLDHEIGHT; i++)						// for all the lines
	{
		printf("|");											// draw the left side of the frame
		for (int j = 0; j < WORLDWIDTH; j++)					// for all the cells in this line
		{
			if (world[j][i] == ALIVE)							// if the cell is alive
			{
				Yellow();										// set the color to yellow
				printf("*");									// draw a *
			}
			else
			{
				Red();											// set the color to red
				printf("-");									// draw a -
			}	
			White();
		}
		printf("|\n");											// draw the right part of the frame
	}

	for (int i = 0; i < WORLDWIDTH + 2; i++) printf("-"); 		// draw the bottom line of the frame

	printf("\n");
}
/* Set the color to Black	*/void Black() 	{ printf("\033[0;30m");}
/* Set the color to Red  	*/void Red() 	{ printf("\033[1;31m");}
/* Set the color to Green	*/void Green() 	{ printf("\033[0;32m");}
/* Set the color to Yellow	*/void Yellow() { printf("\033[0;33m");}
/* Set the color to Blue	*/void Blue() 	{ printf("\033[0;34m");}
/* Set the color to Purple	*/void Purple() { printf("\033[0;35m");}
/* Set the color to Cyan	*/void Cyan() 	{ printf("\033[0;36m");}
/* Set the color to White	*/void White() 	{ printf("\033[0;37m");}
