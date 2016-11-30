#ifndef __M_DATA_H__
#define __M_DATA_H__

typedef struct m_data
{
	char* bytes;
	unsigned long len;
	unsigned long allocated_len;
} m_data;

typedef m_data data_t;

m_data* m_data_new();
void m_data_reserve(m_data* s, unsigned long len);
void m_data_cat_char_array_with_len(m_data* s, void* in, unsigned long in_len);
void m_data_cat_data(m_data* s, m_data* in);
void m_data_free(m_data* s);

#endif
