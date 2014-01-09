#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include "Object.h"
#include "Card.h"
#include "Picture.h"

#pragma warning( disable : 4996 )

Sint16 x, y;
Uint16 type; // 0 = hard/background, 1 = card
int enabled;

struct Picture picture;
struct Card card;

int IS_NULL(struct Object o)
{
	return o.picture.picture == NULL ? 1 : 0;
}

SDL_Rect *Create_Rect_BO(struct Object o)
{
	SDL_Rect *rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	(*rect).h = o.picture.picture->h;
	(*rect).w = o.picture.picture->w;
	(*rect).x = o.x;
	(*rect).y = o.y;
	return rect;
}