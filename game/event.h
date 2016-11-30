#ifndef __M_EVENT_H__
#define __M_EVENT_H__

#define keyboard_event_down 0
#define keyboard_event_up 1
#define keyboard_event_hold 2

typedef struct keyboard_event_t
{
  int key;
  int state;
} keyboard_event_t;

#define touch_event_begin 0
#define touch_event_move 1
#define touch_event_end 2

typedef struct touch_event_t
{
  int x;
  int y;
  int state;
  int location;
} touch_event_t;

#endif
