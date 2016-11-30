#include "game.h"
#include "graphic.h"
#include "model.h"
#include "io.h"
#include "conv.h"
#include "m_string.h"
#include "shader.h"
#include "material.h"
#include "scene.h"

game_t* game_new()
{
	game_t* game = malloc(sizeof(game_t));
 	game->scene = scene_new();
	return game;
}

void game_loop(game_t* game)
{
	/* update scene with fixed time step */
	scene_update(game->scene, 1.0f / 60);
	/* render scene */
	scene_render(game->scene);
}

void game_input_keyboard(game_t* game, keyboard_event_t e)
{
	scene_input_keyboard(game->scene, e);
}

void game_input_touch(game_t* game, touch_event_t e)
{
	scene_input_touch(game->scene, e);
}

void game_free(game_t* game)
{
	scene_free(game->scene);
	free(game);
}
