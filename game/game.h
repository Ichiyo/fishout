#ifndef __M_GAME_H__
#define __M_GAME_H__

#include "scene.h"
#include "event.h"

typedef struct game_t
{
  scene_t* scene;
} game_t;

game_t* game_new();
void game_loop(game_t* game);
void game_input_keyboard(game_t* game, keyboard_event_t e);
void game_input_touch(game_t* game, touch_event_t e);
void game_free(game_t* game);

#endif
