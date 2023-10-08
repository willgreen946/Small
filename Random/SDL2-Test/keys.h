#ifndef KEYS_H
#define KEYS_H

#include <SDL2/SDL.h>

typedef struct {
	int up, down, left, right;
}KEYS;

KEYS keys;

void handle_keypress_up (SDL_KeyboardEvent *event);
void handle_keypress_down (SDL_KeyboardEvent *event);

#include "keys.c"
#endif // KEYS_H
