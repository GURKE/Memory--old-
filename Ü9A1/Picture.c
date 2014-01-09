#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include "Picture.h"

SDL_Surface *load_img(const char *filename);

int PictureID = 0;

SDL_Color textColor = { 255, 255, 255 };
SDL_Color textColorH = { 200, 200, 0 };

#define COLOR_KEY_R   0
#define COLOR_KEY_G   0
#define COLOR_KEY_B   0

int ID;
SDL_Surface *picture;
char filename[1000];

struct Picture load_picture(struct Picture p, char Filename[100])
{
	p.ID = PictureID++;
	strncpy(p.filename, Filename, 100);
	p.picture = load_img(Filename);
	return p;
}

struct Picture Create_Picture_By_Text(struct Picture p, char Text[1000], int Highlighted)
{
	p.ID = PictureID++;
	strncpy(p.filename, Text, 1000);
	if (Highlighted)
		p.picture = TTF_RenderText_Solid(font, Text, textColorH);
	else
		p.picture = TTF_RenderText_Solid(font, Text, textColor);
	return p;
}

SDL_Surface * load_img(const char *filename) {
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	unsigned int colorkey = 0;

	loadedImage = SDL_LoadBMP(filename);

	if (loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	if (optimizedImage != NULL) {
		colorkey = SDL_MapRGB(optimizedImage->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);

		SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
	}

	return optimizedImage;
}