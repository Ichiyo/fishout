#ifndef __M_XML_H__
#define __M_XML_H__

#include "m_string.h"
#include "m_array.h"

typedef struct xml_attribute
{
	m_string* name;
	m_string* value;
} xml_attribute;

typedef struct xml_element
{
	m_string* name;
	m_string* value;
	m_array* attributes;
	m_array* children;
	struct xml_element* parent;
} xml_element;

typedef struct xml_context
{
	/* xml file may contain more than one root so context needs keep array */
	m_array* elements;
} xml_context;

xml_context* parse_xml_file(char* file);
void xml_context_free(xml_context* context);
xml_element* xml_context_find(xml_element* root, char* name, int index);
xml_attribute* xml_context_find_attribute(xml_element* elm, char* name);
xml_element* xml_context_find_with_attribute(xml_element* root, char* name, char* attr, char* attr_val);
#endif
