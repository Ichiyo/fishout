#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int main()
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

	game_t* game = game_new();
	int key_up = 1;
	/* main loop */
	while (1)
	{
		/* process exit event */
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;

			if (windowEvent.type == SDL_KEYUP)
			{
				key_up = 1;
				keyboard_event_t e =
				{
					windowEvent.key.keysym.sym,
					keyboard_event_up
				};
        game_input_keyboard(game, e);
      }
			if(windowEvent.type == SDL_KEYDOWN)
			{
			 	keyboard_event_t e =
				{
					windowEvent.key.keysym.sym,
					key_up ? keyboard_event_down : keyboard_event_hold
				};
				key_up = 0;
				game_input_keyboard(game, e);
			}
		}
		/* run game loop */
		game_loop(game);

		/* swap window buffer */
		SDL_GL_SwapWindow(window);
	}
	game_free(game);
	texture_free_cache();
	/* free gl context */
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
