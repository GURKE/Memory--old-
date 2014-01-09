#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include <memory.h>

#pragma warning( disable : 4996 )

int PictureID = 0;

TTF_Font *font = NULL;
SDL_Color textColor = { 255, 255, 255 };
SDL_Color textColorH = { 200, 200, 0 };

SDL_Surface *load_img(const char *filename);

#define COLOR_KEY_R   0
#define COLOR_KEY_G   0
#define COLOR_KEY_B   0

typedef struct Picture
{
	int ID;
	SDL_Surface *picture;
	char filename[1000];

	void load_picture(char Filename[100])
	{
		ID = PictureID++;
		strncpy(filename, Filename, 100);
		picture = load_img(Filename);
	}

	void Create_picture_By_Text(char Text[1000], int Highlighted)
	{
		ID = PictureID++;
		strncpy(filename, Text, 1000);
		if (Highlighted)
			picture = TTF_RenderText_Solid(font, Text, textColorH);
		else
			picture = TTF_RenderText_Solid(font, Text, textColor);
	}

	Picture() {};
} Picture;

typedef struct Card
{
	Uint16 difficulty; // 0 = easy ... 3 = hard
	Uint16 type; // Pair-ID
	bool seeable = 0; // 0 = background of the card, 1 = foreground
	Picture *picture;

	Card() {};
} Card;

typedef struct Object
{
	Sint16 x, y;
	Uint16 type; // 0 = hard/background, 1 = card
	bool enabled = 1;

	Picture picture;
	Card card;

	bool IS_NULL()
	{
		return picture.picture == NULL ? true : false;
	}

	SDL_Rect Create_Rect()
	{
		SDL_Rect rect;
		rect.h = picture.picture->h;
		rect.w = picture.picture->w;
		rect.x = x;
		rect.y = y;
		return rect;
	}

	Object() {};
} Object;

SDL_Surface * load_img(const char *filename) {
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	unsigned int colorkey = 0;

	loadedImage = SDL_LoadBMP(filename);

	if (loadedImage != NULL) {
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	if (optimizedImage != NULL) {
		colorkey = SDL_MapRGB(optimizedImage->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);

		SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
	}

	return optimizedImage;
}