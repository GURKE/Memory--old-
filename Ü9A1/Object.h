#ifndef _OBJECT_H_
#define _OBJECT_H_

#pragma warning( disable : 4996 )

struct Object
{
	Object() {};

	Sint16 x, y;
	Uint16 type; // 0 = hard/background, 1 = card
	bool enabled = true;

	struct Picture picture;
	struct Card card;

	bool IS_NULL();
	SDL_Rect Create_Rect();
};

#endif