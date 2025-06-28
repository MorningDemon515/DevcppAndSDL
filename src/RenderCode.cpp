#include "Main.h"

#include "Font_Text.h"

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Event event;
extern bool run;

SDL_Surface *Image_Load(const char *filename);

Font_Text ft = Font_Text("C:/Windows/Fonts/simfang.ttf");

void InitCode()
{
	SDL_Color color = {255,255,255};
	ft.Init(32.0f,L"你好",color);
}

void RenderCode()
{
	if(event.type == SDL_KEYDOWN)
	{
		if(event.key.keysym.sym == SDLK_ESCAPE)
		{
			run = false;
		}

	}
	
	SDL_SetRenderDrawColor(Renderer,155,155,155,255);
	SDL_RenderClear(Renderer);
	
	if(event.type == SDL_MOUSEMOTION)
	{
		int x, y;
		x = event.motion.x;
		y = event.motion.y;
		ft.Draw(x,y);
	}
	
	SDL_RenderPresent(Renderer);
}

void DestroyCode()
{

}
