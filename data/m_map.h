#ifndef __M_MAP_H__
#define __M_MAP_H__

#include "m_array.h"

typedef struct m_map
{
	m_array* keys;
	m_array* datas;
} m_map;

m_map* m_map_new(unsigned long item_size);
void m_map_set(m_map* map, void* key, unsigned long key_size, void* p);
int m_map_contain_key(m_map* map, void* key, unsigned long key_size);
void* m_map_get_pointer(m_map* map, void* key, unsigned long key_size);
void m_map_free(m_map* map);

/* generic get data of type by key
	 e.g : 
	 m_map* map = m_map_new(sizeof(float));
	 ..
	 float ret = m_map_get(map, float, qlkey("1234"));
	 
	 e.g :
	 m_map* map = m_map_new(sizeof(float));
	 ..
	 int key = 10;
	 float ret = m_map_get(map, float , qpkey(key));
	 
	 e.g :
	 m_map* map = m_map_new(sizeof(float));
	 ..
	 m_string* s;
	 ..
	 float ret = m_map_get(map, float, qskey(s));
*/
#define m_map_get(m, type, key...) (*(type*)m_map_get_pointer(m,key))

/* quick key and key_size from string literal 
	 e.g : 
	 qlkey("1234") ~ &"1234",sizeof("1234")-1
*/
#define qlkey(s) &s,sizeof(s)-1

/* quick key and key_size from data
	 e.g : 
	 int key = 10;
	 qpkey(key) ~ &key,sizeof(key)	
*/
#define qpkey(p) &p,sizeof(p)

/* quick key and key_size from m_string
	 e.g : 
	 m_string* s;
	 ..
	 qskey(s) ~ s->str,s->len
*/
#define qskey(s) s->str,s->len

#endif
