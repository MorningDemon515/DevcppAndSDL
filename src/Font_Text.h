#ifndef FONT_TEXT
#define FONT_TEXT

#include <SDL2\SDL.h>

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include "stb_truetype.h"

class Font_Text {
	int utf8_next(const wchar_t** str) {
		if (!str || !*str || !**str) 
			return 0;
		
		wchar_t c = **str;
		(*str)++;

#if WCHAR_MAX <= 0xFFFF 
		if (c >= 0xD800 && c <= 0xDBFF) {
			if (!**str) 
				return -1; 
			
			wchar_t c2 = **str;
			if (c2 < 0xDC00 || c2 > 0xDFFF) 
				return -1; 
			
			(*str)++;
			return 0x10000 + ((c - 0xD800) << 10) + (c2 - 0xDC00);
		}
		
		if (c >= 0xDC00 && c <= 0xDFFF) 
			return -1;
		
#endif
		
		return c;
	}
	
	
	 unsigned char* fontBuffer;
	
	SDL_Texture* texture;
	SDL_Surface* surface;
	
public:
	Font_Text(){};
	Font_Text(const char* fontPath);
	~Font_Text();
	
	void Init(float fontSize, const std::wstring& text,SDL_Color color);
	void Draw(int x, int y);

};

#endif
