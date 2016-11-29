#include "m_data.h"
#include <string.h>
#include <stdlib.h>

m_data* m_data_new()
{
	m_data* ret = malloc(sizeof(m_data));
	ret->bytes = 0;
	ret->len = 0;
	ret->allocated_len = 0;
	return ret;
}

void m_data_reserve(m_data* s, unsigned long len)
{
	if(len > s->allocated_len)
	{
		s->allocated_len = len;
		s->bytes = realloc(s->bytes, s->allocated_len * sizeof(char));
	}
}

void m_data_cat_char_array_with_len(m_data* s, void* in, unsigned long in_len)
{
	unsigned long new_len = s->len + in_len;
	m_data_reserve(s, new_len);
	memcpy(s->bytes + s->len, in, in_len);
	s->len += in_len;
}

void m_data_cat_data(m_data* s, m_data* in)
{
	unsigned long new_len = s->len + in->len;
	m_data_reserve(s, new_len);
	memcpy(s->bytes + s->len, in, in->len);
	s->len += in->len;
}

void m_data_free(m_data* s)
{
	free(s->bytes);
	free(s);
}
