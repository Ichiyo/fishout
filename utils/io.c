#include "io.h"
#include "platform.h"

#if TARGET_PLATFORM != PLATFORM_IOS

FILE* io_open_file(const char* file, char* mode)
{
	return fopen(file, mode);
}

#endif

#define io_read_block_size 4096

m_string* io_file_to_string(const char* path)
{
	FILE* file = io_open_file(path, "r");
	if(!file) return 0;

	m_string* ret = m_string_new();
	char buff[io_read_block_size];
	int count = 0;

	while((count = fread(buff, 1, io_read_block_size, file)))
	{
		count -= count == io_read_block_size ? 0 : 1;
		m_string_cat_char_array_with_len(ret, buff, count);
	}

	fclose(file);
	return ret;
}
