#include "Main.h"

SDL_Window* Window;
SDL_Renderer* Renderer;
SDL_Event event;
bool run = true;

void InitCode();
void RenderCode();
void DestroyCode();

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	Window = SDL_CreateWindow(" ",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOWWIDTH,
		WINDOWHEIGHT,
		SDL_WINDOW_SHOWN);
	
	Renderer = SDL_CreateRenderer(Window,1,SDL_RENDERER_ACCELERATED);
	
	InitCode();
	
	SDL_RendererInfo info = {};
	int count = SDL_GetNumRenderDrivers();
	for (int i = 0; i < count; ++i) {
		SDL_GetRenderDriverInfo(i,&info);
		const char* name = info.name;
		printf("Render driver: %d: %s\n", i, name);
	}
	
	while(run){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
			{
				run = false;
			}
			else
			{
				RenderCode();
			}
		}
	}
	
	DestroyCode();
	
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
