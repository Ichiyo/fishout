#ifndef __M_IO_H__
#define __M_IO_H__

#include "m_string.h"
#include <stdio.h>

FILE* io_open_file(const char* file, char* mode);
m_string* io_file_to_string(const char* file);

#endif
