#ifndef KBD_H
#define KBD_H

#include <limits.h>

unsigned char kbd_map[UCHAR_MAX + 1];

/*
 * Stores a state for each key. The meaning of each state
 * (0, 1, etc) is left to the user.
 */
int kbd_state[UCHAR_MAX + 1];

/*
 * Set the keyboard map from the data in the file referenced by path.
 * Used to support different keyboard layouts, so note that qwerty
 * should be the identity map.
 *
 * The referenced map should be such that the ith byte will be the new
 * value of kbd_map[i].
 *
 * A return value of 0 indicates success, error otherwise.
 * On error, the map is set to the identity before returning.
 */
int kbd_setmap (char *path);

#endif /* KBD_H */
