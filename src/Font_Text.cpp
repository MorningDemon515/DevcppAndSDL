#define STB_TRUETYPE_IMPLEMENTATION

#include "Main.h"
#include "Font_Text.h"

#include <vector>
#include <string>
#include <iomanip>

#include <codecvt>
#include <locale>

extern SDL_Renderer* Renderer;

Font_Text::Font_Text(const char* fontPath)
{
	FILE* fontFile = fopen(fontPath, "rb");
	if (!fontFile) {
		SDL_Log("Failed to open font: %s", fontPath);
	}
	
	fseek(fontFile, 0, SEEK_END);
	long size = ftell(fontFile);
	fseek(fontFile, 0, SEEK_SET);
	
	fontBuffer = new unsigned char[size];
	fread(fontBuffer, 1, size, fontFile);
	fclose(fontFile);
}

Font_Text::~Font_Text()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	delete[] fontBuffer;
}

void Font_Text::Init(float fontSize, const std::wstring& text,SDL_Color color)
{
	stbtt_fontinfo font;
	if (!stbtt_InitFont(&font, fontBuffer, 0)) {
		SDL_Log("Failed to load font");
	}
	
	float scale = stbtt_ScaleForPixelHeight(&font, fontSize);
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
	int baseline = (int)(ascent * scale);
	
	std::vector<int> codepoints;
	const wchar_t* p = text.c_str();
	while (*p) {
		int cp = utf8_next(&p);
		if (cp > 0) codepoints.push_back(cp);
	}
	
	int textWidth = 0;
	for (size_t i = 0; i < codepoints.size(); ++i) {
		int advance, lsb;
		stbtt_GetCodepointHMetrics(&font, codepoints[i], &advance, &lsb);
		textWidth += (int)(advance * scale);
		
		if (i < codepoints.size() - 1) {
			textWidth += (int)(stbtt_GetCodepointKernAdvance(
				&font, codepoints[i], codepoints[i+1]) * scale);
		}
	}
	
	int textHeight = baseline + (int)(-descent * scale) + 5;

	surface = SDL_CreateRGBSurfaceWithFormat(
		0, textWidth, textHeight, 32, SDL_PIXELFORMAT_RGBA32
		);
	SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
	SDL_LockSurface(surface);
	
	int x = 0;
	for (size_t i = 0; i < codepoints.size(); ++i) {
		int w, h, xoff, yoff;
		unsigned char* bitmap = stbtt_GetCodepointBitmap(
			&font, scale, scale, codepoints[i], &w, &h, &xoff, &yoff
			);
		
		for (int row = 0; row < h; row++) {
			for (int col = 0; col < w; col++) {
				Uint8 alpha = bitmap[row * w + col];
				if (alpha == 0) continue;
				
				int ypos = baseline + yoff + row;
				if (ypos < 0 || ypos >= textHeight) continue;
				
				Uint32* pixel = (Uint32*)((Uint8*)surface->pixels + 
					ypos * surface->pitch + 
					(x + xoff + col) * 4);
				*pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
			}
		}
		
		// Advance position
		int advance, lsb;
		stbtt_GetCodepointHMetrics(&font, codepoints[i], &advance, &lsb);
		x += (int)(advance * scale);
		
		if (i < codepoints.size() - 1) {
			x += (int)(stbtt_GetCodepointKernAdvance(
				&font, codepoints[i], codepoints[i+1]) * scale);
		}
		
		stbtt_FreeBitmap(bitmap, nullptr);
	}
	
	SDL_UnlockSurface(surface);
	texture = SDL_CreateTextureFromSurface(Renderer, surface);
}

void Font_Text::Draw(int x, int y)
{
	SDL_Rect dest = {x, y, surface->w, surface->h};
	SDL_RenderCopy(Renderer, texture, nullptr, &dest);
}


