#ifndef __M_CONV_H__
#define __M_CONV_H__

#include <stdio.h>

#define int_to_chars(var,i) \
  char var[10]; \
  snprintf(var, sizeof(var), "%d", i);

#endif
