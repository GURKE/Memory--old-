#ifndef _PICTURE_H_
#define _PICTURE_H_

#pragma warning( disable : 4996 )

TTF_Font *font;

struct Picture
{
	Picture() {};

	int ID;
	SDL_Surface *picture;
	char filename[1000];

	void load_picture(char Filename[100]);
	void Create_picture_By_Text(char Text[1000], int Highlighted);
};

#endif