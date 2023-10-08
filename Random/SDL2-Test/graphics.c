#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.h"

void prepare_scene (void)
{
	SDL_SetRenderDrawColor(screen.renderer, 0, 0, 255, 255);
	SDL_RenderClear(screen.renderer);
}

void present_scene (void)
{
	SDL_RenderPresent(screen.renderer);
}

/* create the window on the screen */
int create_window (void)
{
	SDL_CreateWindowAndRenderer(
			window_width_max,
			window_height_max,
			SDL_WINDOW_SHOWN,
			&screen.window,
			&screen.renderer);

	if (!screen.window) {
		fprintf(stderr, "ERROR SDL_CreateWindow\n%s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int create_surface (void)
{
	screen.surface = SDL_GetWindowSurface(screen.window);
	if (!screen.surface) {
		fprintf(stderr, "ERROR: SDL_GetWindowSurface\n%s\n", SDL_GetError());
		return -1;
	}

	SDL_UpdateWindowSurface(screen.window);
	return 0;
}

/* loads an image and returns it as a texture */
void image_to_texture (SDL_Texture *texture, int x, int y)
{
	SDL_Rect destination;

	destination.x = x;
	destination.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy(screen.renderer, texture, NULL, &destination);
}

SDL_Texture *load_texture (const char *filename)
{
	SDL_Texture *texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
			"loading texture %s", filename);

	texture = IMG_LoadTexture(screen.renderer, filename);
	return texture;
}
