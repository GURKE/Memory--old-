#ifndef _CARD_H_
#define _CARD_H_

#include "Picture.h"

#pragma warning( disable : 4996 )

struct Card
{
	Uint16 difficulty; // 0 = easy ... 3 = hard
	Uint16 type; // Pair-ID
	int visible; // 0 = background of the card, 1 = foreground
	struct Picture *picture;
};

#endif