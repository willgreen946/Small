#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "keys.h"
#include "player.h"

/* inits player info like the sprite and starting position */
int player_init (void)
{
	player.current_x_pos = 50;
	player.current_y_pos = 50;
	player.current_health = 100;
	player.maximum_health = 255;
	player.sprite = load_texture("assets/player_sprite.png");
	return 0;
}

void check_player_movement (void)
{
	if (keys.up)
		player.current_y_pos -= 5;

	if (keys.down)
		player.current_y_pos += 5;

	if (keys.left)
		player.current_x_pos -= 5;

	if (keys.right)
		player.current_x_pos += 5;
}
