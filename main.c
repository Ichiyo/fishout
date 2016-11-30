#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "texture.h"

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

	// shader_3d_default_param_t param =
	// {
	// 	.has_texture = 1,
	// 	.has_shadow = 1,
	// 	.use_model_index = 0,
	// 	.number_join_ids = 3,
	// 	.number_joins = 20,
	// 	.number_direction_lights = 1,
	// 	.number_point_lights = 1,
	// 	.number_spot_lights = 1
	// };
	// shader_t* s = shader_3d_default_new(param);
	// if(s) shader_free(s);

	//
	// int i;
	// collada_context* ctx = collada_context_new("res/model/kai.dae");
	// m_array* ret = collada_context_parse(ctx);
	// for(i = 0; i < ret->len; i++)
	// {
	//     model_mesh_free(m_array_get(ret, model_mesh*, i));
	// }
	// m_array_free(ret);
	// collada_context_free(ctx);

	game_t* game = game_new();

	/* main loop */
	while (1)
	{
		/* clear screen */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
