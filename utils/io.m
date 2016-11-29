#include "io.h"
#include "platform.h"

#if TARGET_PLATFORM == PLATFORM_IOS
#import <Foundation/Foundation.h>

FILE* io_open_file(const char* path, char* mode)
{
	FILE* file = fopen([[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:path]
                                                              ofType:nil inDirectory:nil] UTF8String], mode);
  return file;
}

#endif
