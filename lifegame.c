/*
 * lifegame.c
 *
 *  Created on:
 *      Author:
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
#define CHAR_DEAD  ' '

/* current cell states of the world */
static int world[WORLDWIDTH][WORLDHEIGHT];

/* next generation cell states */
static int nextstates[WORLDWIDTH][WORLDHEIGHT];

/* functions to write for Part B of lab */
void initialize_world_from_file(const char *filename)
{
	/* TODO: read the state of the world from a file with
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
	for (int x = 0; x < WORLDWIDTH; x++)
		for (int y = 0; y < WORLDHEIGHT; y++)
			world[x][y] = DEAD;

	 FILE* ptr = fopen(filename, "r");
	 char ch;
	 int x = 0 , y = 0;

	 if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    printf("content of this file are \n");

	 do {
        ch = fgetc(ptr);
		if(ch == '\n' && y < WORLDHEIGHT)
		{
			y++;
			x=0;
		}
			
		else if(ch == CHAR_ALIVE)
			world[x][y] = ALIVE;

			
		else if(ch == CHAR_DEAD)
			world[x][y] = DEAD;
		
		if(x < WORLDWIDTH)
			x++;
    
 
        // Checking if character is not EOF.
        // If it is EOF stop eading.
    } while (ch != EOF);
	fclose(ptr);
}

void save_world_to_file(const char *filename)
{
	/* TODO: write the state of the world into a file with
	   name "filename". Assume the file can be created, or if
	   the file exists, overwrite the file. The ith character
	   of the jth line (zero-indexed) describes world[i][j]
	   using the characters CHAR_ALIVE and CHAR_DEAD

	   This file should be readable using the function
	   initialize_world_from_file(filename) above; we can use
	   it to resume a game later
	 */

	FILE *  fic = NULL;
	fic = fopen (filename,"w");
	int state;

	if (fic==NULL)
   { 
	printf("Erreur d’ouverture de fichier\n" );}
	else
   { 
	for (int y = 0; y < WORLDHEIGHT; y++)
	{
		for (int x = 0; x < WORLDWIDTH; x++)	
		{
			state = get_cell_state(x,y);
			if( state == ALIVE)
			{
				fprintf(fic,"*");
			}

			else
				fprintf(fic," ");
			
		}
		fprintf(fic,"\n");
	}
		
	fclose(fic);
   }
}

/* you shouldn't need to edit anything below this line */

/* initializes the world to a hard-coded pattern, and resets
   all the cells in the next generation to DEAD */
void initialize_world(void) // world[collone][ligne] = ALIVE;
{
	int i, j;

	for (i = 0; i < WORLDWIDTH; i++)
		for (j = 0; j < WORLDHEIGHT; j++)
			world[i][j] = nextstates[i][j] = DEAD;

	/* pattern "glider"*/
	world[1][2] = ALIVE;
	world[3][1] = ALIVE;
	world[3][2] = ALIVE;
	world[3][3] = ALIVE;
	world[2][3] = ALIVE; 

	/*
	 |       *
     |     * * 
     |      **
	*/


	/*
	world[1][3] = ALIVE;
	world[2][3] = ALIVE;
	world[3][3] = ALIVE;*/
}

int is_World_Empty(void)
{
	for (int i = 0; i < WORLDWIDTH; i++)
		for (int j = 0; j < WORLDHEIGHT; j++)
			if (world[i][j] == ALIVE)
				return 0;
	return 1;
}

int get_world_width(void)
{
	return WORLDWIDTH;
}

int get_world_height(void)
{
	return WORLDHEIGHT;
}

int get_cell_state(int x, int y)
{
	if (x < 0 || x >= WORLDWIDTH || y < 0 || y >= WORLDHEIGHT)
		return DEAD;
	return world[x][y];
}

void set_cell_state(int x, int y, int state)
{
	if (x < 0 || x >= WORLDWIDTH || y < 0 || y >= WORLDHEIGHT)
	{
		fprintf(stderr, "Error: coordinates (%d,%d) are invalid.\n", x, y);
		abort();
	}
	nextstates[x][y] = state;
}

void finalize_evolution(void)
{
	for (int x = 0; x < WORLDWIDTH; x++)
		for (int y = 0; y < WORLDHEIGHT; y++)
		{
			world[x][y] = nextstates[x][y];
			nextstates[x][y] = DEAD;
		}
}

void output_world(void)
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
void print_world(void)
{
	/*
	printf("%c",201);
	for(int i = 1 ; i < WORLDWIDTH+1 ; i ++)
		printf("%c",205);
	printf("%c",205);*/

	
	for(int i = 0 ; i < WORLDWIDTH+2 ; i ++)
		printf("-");

	printf("\n");

	for(int i = 0 ; i < WORLDHEIGHT ; i ++)
	{
		printf("|");
		for(int j = 0 ; j < WORLDWIDTH ; j ++)
			{
				if(world[j][i] == ALIVE)
					printf("*");
				else
					printf(" ");
			}
		printf("|\n");
	}

	
	for(int i = 0 ; i < WORLDWIDTH+2 ; i ++)
		printf("-");
	
	printf("\n");
}
