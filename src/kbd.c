#include <stdio.h>
#include "kbd.h"

#define len(a) (sizeof (a) / sizeof (*a))

static void id (void) {
  int i;
  for (i = 0; i < len (kbd_map); ++i) kbd_map[i] = i;
}

int kbd_setmap (char *path) {
  FILE *f;
  int ret = 0, c, i;
  id ();

  f = fopen (path, "r");
  if (f) {
    i = 0;
    while (c = fgetc (f), c != EOF && i <= UCHAR_MAX) {
      kbd_map[i++] = c;
    }
  } else ret = 1;
  return ret;
}
