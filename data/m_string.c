#include "m_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "m_array.h"

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

int m_string_contain_char_array(m_string* s, char* search)
{
	unsigned long src_len = strlen(search);
	long start = -2;
	long end = -1;
	long count = 0;
	long first_between = -1;
	unsigned long i;
	for(i = 0; i < s->len; i++)
	{
		if(s->str[i] == search[count])
		{
			if(first_between == -1 && count > 0 && s->str[i] == search[0])
			{
				first_between = i;
			}
			end = i;
			if(count == 0) start = i;
			count++;
			if(count == src_len)
			{
				goto success;
			}
		}
		else
		{
			count = 0;
			if(first_between > 0)
			{
				// jump to previous index matching first character
				i = first_between;
				first_between = -1;
				count++;
				start = i;
				end = i;
			}
			else
			{
				// move to next character
				start = i-1;
				end = i;
				if(s->str[i] == search[count])
				{
					// current index matches first character
					start = end;
					count++;
				}
			}
		}
	}

	if(end - start + 1 == src_len) // case src_len == 1
	{
		goto success;
	}

fail:
	return 0;
success:
	return 1;
}

static void m_string_replace_char_array_inplace(m_string* s, char* search, unsigned long src_len, char* replace, unsigned long rpl_len)
{
	long start = -2;
	long end = -1;
	long count = 0;
	long first_between = -1;
	unsigned long i;
	for(i = 0; i < s->len; i++)
	{
		if(s->str[i] == search[count])
		{
			if(first_between == -1 && count > 0 && s->str[i] == search[0])
			{
				first_between = i;
			}
			end = i;
			if(count == 0) start = i;
			count++;
			if(count == src_len)
			{
				//replace
				memcpy(s->str + start, replace, src_len);
				first_between = -1;
				start = i - 1;
				end = i;
				count = 0;
			}
		}
		else
		{
			count = 0;
			if(first_between > 0)
			{
				// jump to previous index matching first character
				i = first_between;
				first_between = -1;
				count++;
				start = i;
				end = i;
			}
			else
			{
				// move to next character
				start = i-1;
				end = i;
				if(s->str[i] == search[count])
				{
					// current index matches first character
					start = end;
					count++;
				}
			}
		}
	}

	if(end - start + 1 == src_len) // case src_len == 1
	{
		//replace
		memcpy(s->str + start, replace, src_len);
	}
}

static unsigned long m_string_find_occurence(m_string* s, char* search, unsigned long src_len, m_array* rem)
{
	unsigned long occur = 0;
	long start = -2;
	long end = -1;
	long count = 0;
	long first_between = -1;
	unsigned long i;
	for(i = 0; i < s->len; i++)
	{
		if(s->str[i] == search[count])
		{
			if(first_between == -1 && count > 0 && s->str[i] == search[0])
			{
				first_between = i;
			}
			end = i;
			if(count == 0) start = i;
			count++;
			if(count == src_len)
			{
				occur++;
				if(rem->len == rem->allocated_len) m_array_reserve(rem, rem->allocated_len + 10);
				m_array_push(rem, &start);
				m_array_push(rem, &end);
				first_between = -1;
				start = i - 1;
				end = i;
				count = 0;
			}
		}
		else
		{
			count = 0;
			if(first_between > 0)
			{
				// jump to previous index matching first character
				i = first_between;
				first_between = -1;
				count++;
				start = i;
				end = i;
			}
			else
			{
				// move to next character
				start = i-1;
				end = i;
				if(s->str[i] == search[count])
				{
					// current index matches first character
					start = end;
					count++;
				}
			}
		}
	}

	if(end - start + 1 == src_len) // case src_len == 1
	{
		if(rem->len == rem->allocated_len) m_array_reserve(rem, rem->allocated_len + 10);
		m_array_push(rem, &start);
		m_array_push(rem, &end);
		occur++;
	}

	return occur;
}

static void m_string_replace_char_array_double_buffer(m_string* s, char* search, unsigned long src_len, char* replace, unsigned long rpl_len)
{
	unsigned long i;
	/* create rem to remember blocks [start, end] in s */
	m_array* rem = m_array_new(sizeof(unsigned long));
	unsigned long occur = m_string_find_occurence(s, search, src_len, rem);
	/* calculate return size */
	unsigned long ret_size = s->len - occur * src_len + occur * rpl_len;
	m_string* buff = m_string_new();
	m_string_reserve(buff, ret_size);
	/* replace */
	unsigned long index = 0;
	for(i = 0; i < rem->len; i+= 2)
	{
		unsigned long start = m_array_get(rem, unsigned long, i);
		unsigned long end = m_array_get(rem, unsigned long, i + 1);
		if(start > index)
		{
			m_string_cat_char_array_with_len(buff, s->str+index, start - index);
		}
		m_string_cat_char_array_with_len(buff, replace, rpl_len);
		index = end + 1;
	}
	/* fill tail */
	if(index < s->len)
	{
		m_string_cat_char_array_with_len(buff, s->str+index, s->len - index);
	}
	/* free up rem */
	m_array_free(rem);
	/* replace s content by buff */
	s->len = 0;
	m_string_cat_string(s, buff);
	m_string_free(buff);
}

void m_string_replace_char_array(m_string* s, char* search, char* replace)
{
	unsigned long src_size = strlen(search);
	unsigned long rpl_size = strlen(replace);

	if(src_size == rpl_size)
	{
		//in place
		m_string_replace_char_array_inplace(s, search, src_size, replace, rpl_size);
	}
	else
	{
		//double buffer
		m_string_replace_char_array_double_buffer(s, search, src_size, replace, rpl_size);
	}
}

void m_string_free(m_string* s)
{
	free(s->str);
	free(s);
}
