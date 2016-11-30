#include "m_map.h"
#include "m_data.h"
#include <stdlib.h>
#include <string.h>

#define m_map_modulo 10

static __inline__ int hash(char* key, unsigned long key_size)
{
	unsigned long hash = 5381;
	int i;
	for(i = 0; i < key_size; i++)
	{
		hash = ((hash << 5) + hash) + key[i];
	}
	int ret = hash % m_map_modulo;
	return ret;
}

m_map* m_map_new(unsigned long item_size)
{
	int i;
	m_map* map = malloc(sizeof(m_map));
	map->keys = m_array_new(sizeof(m_array*));
	for(i = 0; i < m_map_modulo; i++)
	{
		m_array* keys = m_array_new(sizeof(m_data*));
		m_array_push(map->keys, &keys);
	}
	map->datas = m_array_new(sizeof(m_array*));
	for(i = 0; i < m_map_modulo; i++)
	{
		m_array* datas = m_array_new(item_size);
		m_array_push(map->datas, &datas);
	}
	return map;
}

void m_map_set(m_map* map, void* key, unsigned long key_size, void* p)
{
	int i;
	int data_index = -1;
	int index = hash(key, key_size);
	m_array* keys = m_array_get(map->keys, m_array*, index);
	m_array* datas = m_array_get(map->datas, m_array*, index);

	for(i = 0; i < keys->len; i++)
	{
		m_data* keys_i = m_array_get(keys, m_data*, i);
		if(keys_i->len == key_size && memcmp(key, keys_i->bytes, key_size) == 0)
		{
			data_index = i;
			break;
		}
	}
	if(data_index == -1)
	{
		m_data* d = m_data_new();
		m_data_cat_char_array_with_len(d, key, key_size);
		m_array_push(keys, &d);
		m_array_push(datas, p);
	}
	else
	{
		m_array_set(datas, data_index, p);
	}
}

int m_map_contain_key(m_map* map, void* key, unsigned long key_size)
{
	int i;
	int index = hash(key, key_size);
	m_array* keys = m_array_get(map->keys, m_array*, index);
	for(i = 0; i < keys->len; i++)
	{
		m_data* keys_i = m_array_get(keys, m_data*, i);
		if(keys_i->len == key_size && memcmp(key, keys_i->bytes, key_size) == 0) return 1;
	}

	return 0;
}

void m_map_remove_key(m_map* map, void* key, unsigned long key_size)
{
	int i;
	int index = hash(key, key_size);
	m_array* keys = m_array_get(map->keys, m_array*, index);
	m_array* datas = m_array_get(map->datas, m_array*, index);

	for(i = 0; i < keys->len; i++)
	{
		m_data* keys_i = m_array_get(keys, m_data*, i);
		if(keys_i->len == key_size && memcmp(key, keys_i->bytes, key_size) == 0)
		{
			m_array_remove_index(keys, i);
			m_data_free(keys_i);
			m_array_remove_index(datas, i);
			goto finish;
		}
	}
finish:
	;
}

static void** null_pointer = 0;

void* m_map_get_pointer(m_map* map, void* key, unsigned long key_size)
{
	int i;
	int index = hash(key, key_size);
	m_array* keys = m_array_get(map->keys, m_array*, index);
	m_array* datas = m_array_get(map->datas, m_array*, index);

	for(i = 0; i < keys->len; i++)
	{
		m_data* keys_i = m_array_get(keys, m_data*, i);
		if(keys_i->len == key_size && memcmp(key, keys_i->bytes, key_size) == 0)
		{
			return (char*)datas->data + i * datas->item_size;
		}
	}
	return &null_pointer;
}

void m_map_free(m_map* map)
{
	int i, j;
	for(i = 0; i < map->keys->len; i++)
	{
		m_array* keys = m_array_get(map->keys, m_array*, i);
		for(j = 0; j < keys->len; j++)
		{
			m_data_free(m_array_get(keys, m_data*, j));
		}
		m_array_free(keys);
	}
	m_array_free(map->keys);
	for(i = 0; i < map->datas->len; i++)
	{
		m_array_free(m_array_get(map->datas, m_array*, i));
	}
	m_array_free(map->datas);
	free(map);
}
