#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.h"
#include "player.h"
#include "keys.h"

void event_loop (void)
{
	bool is_running = true;

	player_init();

	while (is_running) {
		prepare_scene();

		if (grab_input()) is_running = false;

		check_player_movement();
		image_to_texture(player.sprite, player.current_x_pos, player.current_y_pos);
		present_scene();
		SDL_Delay(1000/60); /* 60 fps ? */
	}
}

void global_cleanup (void)
{
	SDL_DestroyRenderer(screen.renderer);
	SDL_DestroyWindow(screen.window);
	SDL_Quit();
}

/* the main setup for the program */
int global_main_setup (void)
{
	/* start SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "ERROR: SDL_Init\n%s\n", SDL_GetError());
		return 1;
	}

	/* start SDL image */ 
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		fprintf(stderr, "ERROR: IMG_Init\n%s\n", SDL_GetError());
		return 1;
	}

	if (create_window())
		return 1;

	if (create_surface())
		return 1;

	event_loop();
	global_cleanup();
	return 0;
}

int main (int argc, char *argv[])
{
	return global_main_setup();
}
