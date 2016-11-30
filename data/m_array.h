#ifndef __M_ARRAY_H__
#define __M_ARRAY_H__

/* generic array hold value datas */
typedef struct m_array
{
	void* data;
	unsigned long len;
	unsigned long allocated_len;
	unsigned long item_size;
} m_array;

typedef m_array array_t;

m_array* m_array_new(unsigned long item_size);
void m_array_reserve(m_array* a, unsigned long len);
void m_array_push(m_array* a, void* p);
void m_array_set(m_array* a, unsigned long index, void* p);
void m_array_copy(m_array* a, unsigned long index, void* out);
void m_array_remove_index(m_array* a, unsigned long index);
void m_array_free(m_array* a);

#define m_array_get(a, type, index) (((type*)a->data)[index])

#endif
