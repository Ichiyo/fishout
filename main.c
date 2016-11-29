#include "game.h"
#include "m_string.h"

int main()
{
	//game_init();
	// int i;
	// collada_context* ctx = collada_context_new("res/sphere.dae");
	// m_array* ret = collada_context_parse(ctx);
	// for(i = 0; i < ret->len; i++)
	// {
	// 	model_mesh_free(m_array_get(ret, model_mesh*, i));
	// }
	// m_array_free(ret);
	// collada_context_free(ctx);
	m_string* s = m_string_new_from_char_array("[trys[][tr[[[[try[[][trys[][s[sd[t[r[[ttt[trys]]]]]]]]]]]]]]][trys]1");
	m_string_replace_char_array(s, "[trys]", "123");
	printf("%s\n",s->str);
	int find = m_string_contain_char_array(s, "1231");
	printf("found : %d\n", find);
	m_string_free(s);
	return 0;
}
