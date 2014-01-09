#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL_ttf.h>
#include <Windows.h>

#include "memory.h"
#include "Card.h"
#include "Object.h"
#include "Picture.h"

#pragma warning( disable : 4996 )
#pragma comment(lib, "winmm.lib") // For the sound

#ifdef __APPLE__
#   include <SDL/SDL.h>
#else
#   include "SDL.h"
#endif

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   1024
#define SCREEN_BPP      32

SDL_Rect Gamefield;

/* function declarations */
int init_game(int AmPlayers, Card stack[], int AmCards, int AmX, int AmY);
int start_game(int amplayers, int card_background_id, Card stack[], int AmCards, int SizeX, int SizeY);
int dist2card(int x, int y, int type);
int paint_screen(int card_background_id);
SDL_Rect Create_Rect(int ID);
char* concat(char *s1, char *s2);
char intToChar(int integer);

Object objects[1000];

int pairs[8];
int textfield[8][2];

int AktPlayer;

/* surfaces declaration - global */
SDL_Surface *_screen = NULL;

Picture *Background;

int start_game(int amplayers, int card_background_id, Card stack[], int AmCards, int SizeX, int SizeY, Picture *BG)
{
	Background = BG;

	SDL_Event event;  /* event handling structure */
	int mod = 0; // 0 = first card, 1 = second card, 2 = turn cards back or remove them
	int card1 = -1;
	int card2 = -1;

	if (amplayers > 8)
		return TOO_MANY_PLAYERS;

	if (init_game(amplayers, stack, AmCards, SizeX, SizeY))
		return FAILED_LOADING_GAME_OBJECTS;

	paint_screen(card_background_id);

	font = TTF_OpenFont("./resources/lazy.ttf", 28);
	if (font == NULL) { return INITIALIZATION_FAILURE; }

	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_QUIT)
			break;
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
		{
			int actcard;
			switch (mod)
			{
			case 0:	case 1: // tap card
				actcard = dist2card(event.button.x, event.button.y, 1);
				if (actcard > -1 && objects[actcard].enabled && !objects[actcard].card.seeable)
				{
					if (mod == 0)
						card1 = actcard;
					else
						card2 = actcard;

					objects[actcard].card.seeable = true;
					mod++;
				}
				break;

			case 2: // Remove them or move them back
				if (objects[card1].card.type == objects[card2].card.type)
				{
					objects[card1].enabled = false;
					objects[card2].enabled = false;

					pairs[AktPlayer]++;

					char *c = (char *)malloc(sizeof(char));
					sprintf(c, "%d", pairs[AktPlayer]);
					objects[textfield[AktPlayer][1]].picture.Create_picture_By_Text(concat(c, " pairs found"), 0);
				}
				else
				{
					objects[card1].card.seeable = false;
					objects[card2].card.seeable = false;

					char *c = (char *)malloc(sizeof(char));
					sprintf(c, "%d", AktPlayer + 1);
					objects[textfield[AktPlayer][0]].picture.Create_picture_By_Text(concat("Player ", c), 0);

					AktPlayer++;
					if (AktPlayer == amplayers) AktPlayer = 0;

					sprintf(c, "%d", AktPlayer + 1);
					objects[textfield[AktPlayer][0]].picture.Create_picture_By_Text(concat("Player ", c), 1);
				}
				mod = 0;
				break;
			default: break;
			}
			paint_screen(card_background_id);
		}
	}
}

int dist2card(int x, int y, int type)
{
	int i = 0;
	while (!objects[i].IS_NULL())
	{
		if (objects[i].enabled && !objects[i].card.seeable && objects[i].type == type)
		{
			double xrel = x - objects[i].x;
			double yrel = y - objects[i].y;
			if (xrel > 0 && xrel < (objects[i].picture).picture->w)
			if (yrel > 0 && yrel < (objects[i].picture).picture->h)
				return i;
		}
		i++;
	}
	return -1;
}

char* concat(char *s1, char *s2)
{
	char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);

	return result;
}

int init_game(int AmPlayers, Card stack[], int AmCards, int AmX, int AmY)
{
	srand(time(NULL));  /* start random number generater */

	// CHANGE THIS HERE I = 0 ISNT CORRECT!
	int i = 0;

	AktPlayer = rand() % (AmPlayers + 1);

	for (int j = 0; j < AmPlayers; j++)
	{
		objects[i].x = j % 2 == 0 ? 10 : 1093;
		objects[i].y = 10 + (j / 2) * 256;
		objects[i].type = 100;
		objects[i].enabled = true;
		char *c = (char *)malloc(sizeof(char));
		sprintf(c, "%d", j + 1);
		objects[i].picture.Create_picture_By_Text(concat("Player ", c), j == AktPlayer ? 1 : 0);

		pairs[j] = 0;
		textfield[j][0] = i++;

		objects[i].x = j % 2 == 0 ? 10 : 1093;
		objects[i].y = 30 + (j / 2) * 256;
		objects[i].type = 101;
		objects[i].enabled = true;
		objects[i].picture.Create_picture_By_Text("0 pairs found", 0);

		pairs[j] = 0;
		textfield[j][1] = i++;
	}


	FILE *f;
	f = fopen("./resources/config.txt", "r");

	if (fscanf(f, "%d %d %d %d", &Gamefield.x, &Gamefield.y, &Gamefield.w, &Gamefield.h) == EOF)
		return -1;


	// Load Objects

	char c[100];
	while (fscanf(f, "%s %3d %3d %3d", &c, &objects[i].x, &objects[i].y, &objects[i].type) != EOF)
	{
		objects[i].picture.load_picture(c);
		if (objects[i].picture.picture == NULL)
			return FAILED_LOADING_IMAGE;

		objects[i].enabled = 1;
		i++;
	}

	// Shuffle Cards in Stack - Knuth-Fisher-Yates shuffle **/
	for (int j = AmCards - 1; j; j--) {
		int k = rand() % (j + 1);  /* random variable modulo remaining cards */
		/* swap entries of fields i and j */
		Card swap = stack[j];
		stack[j] = stack[k];
		stack[k] = swap;
	}

	// Draw Cards on gamefield

	int j = 0;
	int SizeX = (*stack[0].picture).picture->w;
	int SizeY = (*stack[0].picture).picture->h;

	for (int x = 0; x < AmX; x++)
	for (int y = 0; y < AmY; y++)
	{
		if ((*stack[j].picture).picture == NULL)
			break;

		objects[i].type = 1; // objects is card
		objects[i].card = stack[j++];
		objects[i].picture = *objects[i].card.picture;
		objects[i].card.picture = &objects[i].picture;
		objects[i].enabled = true;
		objects[i].x = Gamefield.x + Gamefield.w / AmX * x;
		objects[i].y = Gamefield.y + Gamefield.h / AmY * y;
		i++;
	}

	return 0;
}


int paint_screen(int card_background_id)
{
	_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_FillRect(_screen, NULL, _bg_color);

	int i = 0;
	while (!objects[i].IS_NULL())
	{
		if (objects[i].enabled)
		{
			if (objects[i].type == 1) // card
			{
				if (objects[i].card.seeable)
					SDL_BlitSurface(objects[i].picture.picture, NULL, _screen, &objects[i].Create_Rect()); // Foreground of the card
				else
					SDL_BlitSurface((*Background).picture, NULL, _screen, &objects[i].Create_Rect()); // Background of the card
			}
			else
				SDL_BlitSurface(objects[i].picture.picture, NULL, _screen, &objects[i].Create_Rect());
		}
		i++;
	}

	SDL_Flip(_screen);

	return 0;
}