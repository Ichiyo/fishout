#include "xml.h"
#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define xml_none 0
#define xml_read_element_name 1
#define xml_find_attribute_name 2
#define xml_read_attribute_name 3
#define xml_find_attribute_value 4
#define xml_read_attribute_value 5
#define xml_find_element 6
#define xml_check_element_value 7

static xml_attribute* new_attribute()
{
	xml_attribute* ret = malloc(sizeof(xml_attribute));
	ret->name = m_string_new();
	ret->value = m_string_new();
	return ret;
}

static xml_element* new_element()
{
	xml_element* ret = malloc(sizeof(xml_element));
	ret->children = m_array_new(sizeof(xml_element*));
	ret->attributes = m_array_new(sizeof(xml_attribute*));
	ret->parent = 0;
	ret->name = m_string_new();
	ret->value = m_string_new();
	return ret;
}

xml_context* parse_xml_file(char* file)
{
	int depth = 0;
	unsigned long i;
	unsigned long start = 0;
	unsigned long end = 0;
	int finish = 0;
	int empty_text = 1;
	int state = xml_none;
	/* init context */
	xml_context* context = malloc(sizeof(xml_context));
	context->elements = m_array_new(sizeof(xml_element*));
	/* read xml file */
	m_string* text = io_file_to_string(file);
	/* setup current root */
	xml_element* current = 0;
	xml_attribute* current_attr;

	for(i = 0; i < text->len; i++)
	{
		char c = text->str[i];
		switch(state)
		{
			case xml_none:
			if(c == '<')
			{
				if(text->str[i + 1] == '/')
				{
					i++;
					current = current->parent;
					state = xml_none;
				}
				else
				{
					if(current)
					{
						xml_element* child = new_element();
						m_array_push(current->children, &child);
						child->parent = current;
						current = child;
					}
					else
					{
						current = new_element();
						m_array_push(context->elements, &current);
					}
					start = i + 1;
					end = i;
					state = xml_read_element_name;
					finish = 0;
				}
			}
			break;

			case xml_read_element_name:
			if(isspace(c))
			{
				//finish read element name
				finish = 1;
				state = xml_find_attribute_name;
			}
			else
			{
				if(c == '>' || c == '?')
				{
					//finish read element name
					finish = 1;
					state = xml_check_element_value;
					empty_text = 1;
					if(c == '?') i++; // workaround for ?> case
				}
				else if(c == '/')
				{
					// end node
					finish = 1;
					state = xml_none;
				}
				else
				{
					//increase end index of element name
					end++;
				}
			}

			if(finish)
			{
				if(end >= start)
				{
					m_string_cat_char_array_with_len(current->name, text->str + start, end - start + 1);
				}
				finish = 0;
				start = end = 0;

				if(state == xml_none)
				{
					current = current->parent;
				}
			}
			break;

			case xml_find_attribute_name:
			if(c == '>' || c == '?')
			{
				// finish read attributes, jump to check element value
				state = xml_check_element_value;
				start = end = 0;
				empty_text = 1;
				if(c == '?') i++; // workaround for ?> case
			}
			else if(c == '/')
			{
				current = current->parent;
				state = xml_none;
			}
			else if(!isspace(c))
			{
				//begin read attribute name
				state = xml_read_attribute_name;
				start = end = i;
				finish = 0;
			}
			break;

			case xml_read_attribute_name:
			if(isspace(c) || c == '=')
			{
				//finish read attribute name
				current_attr = new_attribute();
				m_string_cat_char_array_with_len(current_attr->name, text->str + start, end - start + 1);
				m_array_push(current->attributes, &current_attr);
				start = end = 0;
				finish = 0;
				state = xml_find_attribute_value;
			}
			else
			{
				end++;
			}
			break;

			case xml_find_attribute_value:
			if(c == '>' || c == '?')
			{
				// finish read attributes, jump to check element value
				state = xml_check_element_value;
				start = end = 0;
				empty_text = 1;
				if(c == '?') i++; // workaround for ?> case
			}
			else if(c == '\"' || c == '\'')
			{
				//begin read attribute name
				state = xml_read_attribute_value;
				start = i + 1;
				end = i;
				finish = 0;
			}
			break;

			case xml_read_attribute_value:
			if(c == '\"' || c == '\'')
			{
				//finish read attribute name
				if(end >= start) m_string_cat_char_array_with_len(current_attr->value, text->str + start, end - start + 1);
				start = end = 0;
				finish = 0;
				state = xml_find_attribute_name;
			}
			else
			{
				end++;
			}
			break;

			case xml_find_element:
			if(c == '/')
			{
				//find end element
				current = current->parent;
				state = xml_none;
			}
			else
			{
				/* create new child and assign it to current parent */
				start = end = i;
				finish = 0;
				state = xml_read_element_name;
				xml_element* child = new_element();
				m_array_push(current->children, &child);
				child->parent = current;
				current = child;
			}
			break;

			case xml_check_element_value:
			if(c == '<')
			{
				//finish read element value
				if(!empty_text)
				{
					//element has value
					m_string_cat_char_array_with_len(current->value, text->str + start, end - start + 1);
				}
				state = xml_find_element;
				start = end = 0;
				finish = 0;
			}
			else
			{
				if(start == 0) start = i;
				end = i;
				if(!isspace(c)) empty_text = 0;
			}
			break;
		}
	}

	m_string_free(text);
	return context;
}

static void free_attribute(xml_attribute* attr)
{
	/* free name and value */
	m_string_free(attr->name);
	m_string_free(attr->value);
	/* free self */
	free(attr);
}

static void free_element(xml_element* elm)
{
	int i;
	/* free name and value */
	m_string_free(elm->name);
	m_string_free(elm->value);
	/* free attributes */
	for(i = 0; i < elm->attributes->len; i++)
	{
		free_attribute(m_array_get(elm->attributes, xml_attribute*, i));
	}
	m_array_free(elm->attributes);
	/* free children */
	for(i = 0; i < elm->children->len; i++)
	{
		free_element(m_array_get(elm->children, xml_element*, i));
	}
	m_array_free(elm->children);
	/* free self */
	free(elm);
}

static xml_element* xml_context_find_s(xml_element* root, char* name, int* index)
{
	int i;
	for(i = 0; i < root->children->len; i++)
	{
		xml_element* child = m_array_get(root->children, xml_element*, i);
		if(strcmp(child->name->str, name) == 0)
		{
			if(*index == 0) return child;
			(*index)--;
		}

		xml_element* try = xml_context_find_s(child, name, index);
		if(try) return try;
	}
	return 0;
}

xml_element* xml_context_find(xml_element* root, char* name, int index)
{
	return xml_context_find_s(root, name, &index);
}

xml_attribute* xml_context_find_attribute(xml_element* elm, char* name)
{
	int i;
	for(i = 0; i < elm->attributes->len; i++)
	{
		xml_attribute* att = m_array_get(elm->attributes, xml_attribute*, i);
		if(strcmp(att->name->str, name) == 0) return att;
	}
	return 0;
}

xml_element* xml_context_find_with_attribute(xml_element* root, char* name, char* attr, char* attr_val)
{
	int i;
	for(i = 0; i < root->children->len; i++)
	{
		xml_element* child = m_array_get(root->children, xml_element*, i);
		if(strcmp(child->name->str, name) == 0)
		{
			xml_attribute* a = xml_context_find_attribute(child, attr);
			if(a && strcmp(a->value->str, attr_val) == 0) return child;
		}

		xml_element* try = xml_context_find_with_attribute(child, name, attr, attr_val);
		if(try) return try;
	}
	return 0;
}

void xml_context_free(xml_context* context)
{
	int i;
	/* free all roots */
	for(i = 0; i < context->elements->len; i++)
	{
		free_element(m_array_get(context->elements, xml_element*, i));
	}
	m_array_free(context->elements);
	/* free self */
	free(context);
}
