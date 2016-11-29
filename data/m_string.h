#ifndef __M_STRING_H__
#define __M_STRING_H__

typedef struct m_string
{
	char* str;
	unsigned long len;
	unsigned long allocated_len;
} m_string;

m_string* m_string_new();
m_string* m_string_new_from_char_array(char* in);
m_string* m_string_new_from_string(m_string* in);
void m_string_reserve(m_string* s, unsigned long len);
void m_string_cat_char_array(m_string* s, char* in);
void m_string_cat_char_array_with_len(m_string* s, char* in, unsigned long in_len);
void m_string_cat_string(m_string* s, m_string* in);
void m_string_free(m_string* s);

#endif
