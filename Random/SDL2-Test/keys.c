#include <SDL2/SDL.h>
#include "keys.h"

/* decides what to do with keypresses */
void handle_keypress_down (SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)	{

		if (event->keysym.scancode == SDL_SCANCODE_UP)
			keys.up = 1;

		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
			keys.down = 1;

		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
			keys.left = 1;

		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
			keys.right = 1;
	}
}

/* decides what to do with key releases */
void handle_keypress_up (SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)	{

		if (event->keysym.scancode == SDL_SCANCODE_UP)
			keys.up = 0;

		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
			keys.down = 0;

		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
			keys.left = 0;

		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
			keys.right = 0;
	}
}

/* handles the input events for SDL2 */
int grab_input (void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event) > 0)
			switch (event.type) {
				case SDL_KEYDOWN:
					handle_keypress_down(&event.key);
					break;
				case SDL_KEYUP:
					handle_keypress_up(&event.key);
					break;
				case SDL_QUIT:
					return 1;
					break;
				default:
					break;
			}

	return 0;
}
