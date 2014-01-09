#ifndef _PICTURE_H_
#define _PICTURE_H_

#pragma warning( disable : 4996 )

TTF_Font *font;

struct Picture
{
	int ID;
	SDL_Surface *picture;
	char filename[1000];
};

extern struct Picture load_picture(struct Picture p, char Filename[100]);
extern struct Picture Create_Picture_By_Text(struct Picture p, char Text[1000], int Highlighted);

#endif