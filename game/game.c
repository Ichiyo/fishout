#include "game.h"
#include "graphic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

void game_init()
{
	/* create window */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window* window = SDL_CreateWindow("Game", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_Event windowEvent;
	
	/* main loop */
	while (1)
	{
		/* clear screen */
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		/* process exit event */
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
			  windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}
		/* run game loop */
		game_loop();
			
		/* swap window buffer */
		SDL_GL_SwapWindow(window);
	}
	
	/* free gl context */
	SDL_GL_DeleteContext(context);
	SDL_Quit();
}

void game_loop()
{

}

void game_shutdown()
{

}
