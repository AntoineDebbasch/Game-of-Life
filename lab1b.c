/*
 * lab1b.c
 *
 *  Created on: 04/10/2022
 *      Author: Antoine DEBBASCH
 */

/* include helper functions for game */
#include "lifegame.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <termios.h>

// gcc lab1b.c lifegame.c -o output -g -Wall; ./output

/* number of generations to evolve the world */
#define NUM_GENERATIONS 10

/* this function set the state of all
   the cells in the next generation and call
   finalize_evolution() to update the current
   state of the world to the next generation */
void next_generation(void);

/* this function  return the state of the cell
   at (x,y) in the next generation, according to the
   rules of Conway's Game of Life (see handout) */
int get_next_state(int x, int y);

/* this function calculate the number of alive
   neighbors of the cell at (x,y) */
int num_neighbors(int x, int y);

/* this function let the user chose the initial configuration
 */
void menu(int ch_menu);

/* this function give the number of txt files and the number of char in total for the dynamic alloc
 */
void get_info_txt(int *cpt_fic, int *nbchar);

/* this function is used like a scanf without the need to press enter
 */
int getch(void);

/* this function let the user visualize the file he is about to open
 */
void visualize_file(const char *filename);

int main(int argc, char **argv)
{

	/*
		int a = getch();
		int fleche = 0;
		int cpt = 0;
		while(a != 10)
		{
			a = getch();
			if(fleche == 1)
			{
				if(a==65) cpt++; // printf("Haut");
				if(a==66) cpt--; //printf("Bas");
				system("tput clear");
				fleche = 0;
			}

			if(a == 91)
			{
				//printf("fleche\n");
				fleche = 1;
			}
			else
			{
				//printf("%d\n",a);
				fleche = 0;
			}

			//system("tput clear");
			printf("%d  ",cpt);



		}


		return 0;*/

	menu(3);

	printf("Generation n : 0\n");
	print_world();
	for (int n = 0; n < NUM_GENERATIONS; n++)
	{
		// system("tput clear");

		printf("Generation n : %d\n", n + 1);
		next_generation();
		print_world();
		usleep(300000);

		if (is_World_Empty())
			n = NUM_GENERATIONS;
	}

	/* TODO: output final world state to console and save
	   world to file "Final.txt" in current directory */

	puts("Hello World !!");
	save_world_to_file("./txt/Final.txt");
	return 0;
}

void next_generation(void)
{
	/* DONE: for every cell, set the state in the next
	   generation according to the Game of Life rules

	   Hint: use get_next_state(x,y) */
	for (int x = 0; x < get_world_width(); x++)
		for (int y = 0; y < get_world_height(); y++)
			set_cell_state(x, y, get_next_state(x, y));

	finalize_evolution(); /* called at end to finalize */
}

int get_next_state(int x, int y)
{
	/* DONE: for the specified cell, compute the state in
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
	/* DONE: for the specified cell, return the number of
	   neighbors that are ALIVE

	   Use get_cell_state(x,y) */

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
void menu(int ch_menu)
{

	system("tput clear");
	int cpt_fic = 0;
	int nbchar = 0;

	char **tab;
	char path[20] = "./txt/";
	int key;

	get_info_txt(&cpt_fic, &nbchar);

	struct dirent *dir;
	DIR *d2 = opendir("./txt");

	tab = (char **)malloc(cpt_fic * sizeof(char *) * nbchar);
	cpt_fic = 0;

	if (d2)
	{
		while ((dir = readdir(d2)) != NULL)
		{
			tab[cpt_fic] = (char *)malloc(strlen(dir->d_name) * sizeof(char));
			strcpy(tab[cpt_fic], dir->d_name);
			cpt_fic++;
		}
		closedir(d2);
	}

	printf("Wich file do you want to open ?\n\n");

	for (int i = 2; i < cpt_fic; i++)
	{
		printf("%d : %s\n", i - 1, tab[i]);
		if (ch_menu == i-1)
		{
			puts("------------------------------------------");
			visualize_file(strcat(path, tab[i]));
			puts("------------------------------------------");
			strcpy(path, "./txt/");
		}
	}

	printf("\n\nWich file do you want to open (Move with z and s): \n\n");

    key = getch();

	printf("key = %d\n",key);
	usleep(100000);
	
	if(key == 10)
	{
		strcat(path, tab[ch_menu + 1]);
		initialize_world_from_file(path);
		free(tab);
	}
	else
	{
		if((key == 122 || key == 90) && ch_menu >= 2)
			ch_menu--;
			
		if((key == 115 || key == 83) && ch_menu < cpt_fic-2)
			ch_menu++;
						
		menu(ch_menu);
	}
		
	
}

void get_info_txt(int *cpt_fic, int *nbchar)
{
	struct dirent *dir;
	// opendir() renvoie un pointeur de type DIR.
	DIR *d = opendir("./txt");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			(*nbchar) += strlen(dir->d_name);
			(*cpt_fic)++;
		}
		closedir(d);
	}
}

int getch(void)
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}

void visualize_file(const char *filename)
{
	FILE *ptr = fopen(filename, "r");
	char ch;

	if (NULL == ptr)
	{
		printf("file can't be opened \n");
	}

	do
	{
		ch = fgetc(ptr);
		if (ch == '\n')
			puts(" ");

		else if (ch == '*')
			printf("%c", '*');
		else if (ch == ' ')
			printf("%c", ' ');

	} while (ch != EOF);

	fclose(ptr);
	puts(" ");
}