#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "graphics.h"
#include "keys.h"

typedef struct {
	int current_y_pos;
	int current_x_pos;
	int current_health;
	int maximum_health;
	SDL_Texture *sprite;
}PLAYER;

PLAYER player;

/* initialises the default player variables like the maximum health,
 * the starting health, the texture to be loaded */
int player_init (void);

/* checks the players movement and moves if needed */
void check_player_movement (void);

#include "player.c"
#endif // PLAYER_H
