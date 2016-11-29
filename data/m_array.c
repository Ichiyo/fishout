#include "m_array.h"
#include <stdlib.h>
#include <string.h>

m_array* m_array_new(unsigned long item_size)
{
	m_array* ret = malloc(sizeof(m_array));
	ret->data = 0;
	ret->len = 0;
	ret->allocated_len = 0;
	ret->item_size = item_size;
	return ret;
}

void m_array_reserve(m_array* a, unsigned long len)
{
	if(len > a->allocated_len)
	{
		a->allocated_len = len;
		a->data = realloc(a->data, len * a->item_size);
	}
}

void m_array_push(m_array* a, void* p)
{
	m_array_reserve(a, a->len + 1);
	memcpy(a->data + a->len * a->item_size, p, a->item_size);
	a->len += 1;
}

void m_array_set(m_array* a, unsigned long index, void* p)
{
	memcpy(a->data + a->item_size * index, p, a->item_size);
}

void m_array_copy(m_array* a, unsigned long index, void* out)
{
	memcpy(out, a->data + index * a->item_size, a->item_size);
}

void m_array_free(m_array* a)
{
	free(a->data);
	free(a);
}
