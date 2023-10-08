#ifndef GRAPHICS_H
#define GRAPHICS_H

const int window_height_max = 500;
const int window_width_max = 500;

/* This struct contains infomation about the screen and SDL structs */
typedef struct {
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
}SCREEN;

SCREEN screen;

/* This functions prepares to render */
void prepare_scene (void);

/* This function actually displays it */
void present_scene (void);

/* Creates the window in the X session */
int create_window (void);

/* Creates the surface texture */
int create_surface (void);

/* This function draws a specified texture on the screen,
 * at the specified x and y positions */
void image_to_texture (SDL_Texture *texture, int x, int y);

/* Calls SDL image function to load an image and returns it,
 * as a texture */
SDL_Texture *load_texture (const char *filename);

#include "graphics.c"
#endif // GRAPHICS_H
