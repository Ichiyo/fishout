#ifndef __M_CONV_H__
#define __M_CONV_H__

#include <stdio.h>

#define int_to_chars(var,i) \
  char var[10]; \
  snprintf(var, sizeof(var), "%d", i);

#define int_to_chars_style(var,i,style) \
  char var[10]; \
  snprintf(var, sizeof(var), style, i);

#endif
