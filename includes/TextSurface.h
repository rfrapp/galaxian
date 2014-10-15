/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef TEXTSURFACE_H
#define TEXTSURFACE_H

#include "Color.h"
#include <string>



class TextSurface
{ 
public:
	TextSurface( const char *s="Hello, World!", char ff[]="arial.ttf", int size=12, int R=255, int G=255, int B=255 )
				: str(s)
	{
		atexit( TTF_Quit );
		if ( TTF_Init() != 0 )
		{
			printf("TTF_Init: %s\n", TTF_GetError());
			return;
		}

		font = TTF_OpenFont( ff, size );
		color = {R,G,B};
		text = TTF_RenderText_Solid( font, s, color );
	}

	~TextSurface()
	{
		if (text != NULL)
			SDL_FreeSurface( text );
		TTF_CloseFont( font );
	}

	void set_text(std::string t)
	{
		str = t;
		SDL_FreeSurface(text);
		text = TTF_RenderText_Solid(font, str.c_str(), color);
	}

	SDL_Surface * get_surface()
	{
		return text;
	}

private:
	SDL_Surface * text;
	TTF_Font * font;
	Color color;
	std::string str;
};

#endif
