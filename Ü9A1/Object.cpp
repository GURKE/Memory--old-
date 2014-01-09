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
bool enabled = true;

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