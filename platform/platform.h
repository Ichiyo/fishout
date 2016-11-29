#ifndef _M_PLATFORM_H
#define _M_PLATFORM_H

#define PLATFORM_UNKNOWN 0
#define PLATFORM_IOS 1
#define PLATFORM_ANDROID 2
#define PLATFORM_LINUX 3
#define PLATFORM_MAC 4

#define TARGET_PLATFORM PLATFORM_UNKNOWN

#if defined(__APPLE__)

  #include "TargetConditionals.h"
  #if TARGET_IPHONE_SIMULATOR
  #elif TARGET_OS_IPHONE

    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_IOS

  #elif TARGET_OS_MAC
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_MAC
  #endif

#elif defined(linux)

  #undef TARGET_PLATFORM
  #define TARGET_PLATFORM PLATFORM_LINUX

#endif

#endif
