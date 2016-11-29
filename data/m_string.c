#include "m_string.h"
#include <stdlib.h>
#include <string.h>

m_string* m_string_new()
{
	m_string* ret = malloc(sizeof(m_string));
	ret->str = 0;
	ret->len = 0;
	ret->allocated_len = 0;
	return ret;
}

m_string* m_string_new_from_char_array(char* in)
{
	m_string* ret = m_string_new();
	m_string_cat_char_array(ret, in);
	return ret;
}

m_string* m_string_new_from_string(m_string* in)
{
	m_string* ret = m_string_new();
	m_string_cat_string(ret, in);
	return ret;
}

void m_string_reserve(m_string* s, unsigned long len)
{
	len++;
	if(len > s->allocated_len)
	{
		s->allocated_len = len;
		s->str = realloc(s->str, s->allocated_len * sizeof(char));
	}
}

void m_string_cat_char_array(m_string* s, char* in)
{
	unsigned long in_len = strlen(in);
	unsigned long new_len = s->len + in_len;
	m_string_reserve(s, new_len);
	memcpy(s->str + s->len, in, in_len);
	s->len += in_len;
	s->str[s->len] = '\0';
}

void m_string_cat_char_array_with_len(m_string* s, char* in, unsigned long in_len)
{
	unsigned long new_len = s->len + in_len;
	m_string_reserve(s, new_len);
	memcpy(s->str + s->len, in, in_len);
	s->len += in_len;
	s->str[s->len] = '\0';
}

void m_string_cat_string(m_string* s, m_string* in)
{
	unsigned long new_len = s->len + in->len;
	m_string_reserve(s, new_len);
	memcpy(s->str + s->len, in->str, in->len + 1);
	s->len += in->len;
}

void m_string_free(m_string* s)
{
	free(s->str);
	free(s);
}
