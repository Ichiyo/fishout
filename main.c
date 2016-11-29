#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include "xml.h"
#include "m_map.h"
#include "m_string.h"
#include "model.h"

int main()
{
	//game_init();
	int i;
	collada_context* ctx = collada_context_new("res/sphere.dae");
	m_array* ret = collada_context_parse(ctx);
	for(i = 0; i < ret->len; i++)
	{
		model_mesh_free(m_array_get(ret, model_mesh*, i));
	}
	m_array_free(ret);
	collada_context_free(ctx);
	return 0;
}
