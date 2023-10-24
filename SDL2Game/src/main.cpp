#include <iostream>
#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace game {

	class window {
		private:

		SDL_Window * win;
		SDL_Surface * sur;

		public:

		SDL_Renderer * ren;

		void
		destroy(void)
		{
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_Quit();
			exit(0);
		}

		void
		prepare_scene(void)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 0);
			SDL_RenderClear(ren);
		}

		void
		present_scene(void)
		{
			SDL_RenderPresent(ren);
		}

		int
		create_surface(void)
		{
			sur = SDL_GetWindowSurface(win);

			if (!sur) {
				std::cerr << "ERROR:" << SDL_GetError() << '\n';
				return 1;
			}

			SDL_UpdateWindowSurface(win);
			return 0;
		}

		/* Creates a window on the screen */
		int
		create_window(uint width, uint height)
		{
			SDL_CreateWindowAndRenderer(
				width, height,
				SDL_WINDOW_SHOWN,
				&win, &ren);

			if (!win || !ren) {
				std::cerr << "ERROR:" << SDL_GetError() << '\n';
				return 1;
			}

			if (create_surface())
				return 1;

			return 0;
		}
	};

	class player {
		private:

		SDL_Texture * sprite;

		public:

		int x;
		int y;

		void
		render_player(SDL_Renderer * r, int x, int y)
		{
			SDL_Rect destination;

			destination.x = x;
			destination.y = y;

			SDL_QueryTexture(sprite, NULL, NULL, &destination.w, &destination.h);
			SDL_RenderCopy(r, sprite, NULL, &destination);
		}

		SDL_Texture *
		load_texture(const char * asset, SDL_Renderer ** ren)
		{
			SDL_Texture * texture;

			SDL_LogMessage(
				SDL_LOG_CATEGORY_APPLICATION,
				SDL_LOG_PRIORITY_INFO,
				"Loading texure %s", asset);

			texture = IMG_LoadTexture(*ren, asset);
			return texture;
		}

		void
		init(const char * asset, SDL_Renderer ** ren)
		{
			sprite = load_texture(asset, &*ren);
			x = 50;
			y = 50;
		}
	};

	void
	handle_kp_down(SDL_KeyboardEvent * event, game::window & win, game::player & ply)
	{
		if (!event->repeat) {
			switch (event->keysym.scancode) {
				case SDL_SCANCODE_UP:
					ply.y-=8;
					break;
				case SDL_SCANCODE_DOWN:
					ply.y+=8;
					break;
				case SDL_SCANCODE_LEFT:
					ply.x-=8;
					break;
				case SDL_SCANCODE_RIGHT:
					ply.x+=8;
					break;
				case SDL_SCANCODE_Q:
					win.destroy();
					break;
				default:
					break;
			}
		}
	}

	int
	grab_input(game::window & win, game::player & ply)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) > 0) {
			switch (e.type) {
				case SDL_KEYDOWN:
					handle_kp_down(&e.key, win, ply);
					break;
				case SDL_QUIT:
					return 1;
			}
		}

		return 0;
	}

	void
	event_loop(game::window & win, game::player & ply)
	{
		for (;;) {
			win.prepare_scene();
			ply.render_player(win.ren, ply.x, ply.y);

			if (game::grab_input(win, ply)) {
				win.destroy();
				return;
			}

			win.present_scene();
			SDL_Delay(1000/60); // 60 fps?
		}
	}

	int
	setup(void)
	{
		game::window win;
		game::player ply;

		/* Initialising SDL */
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cerr << "ERROR:" << SDL_GetError() << '\n';
			return 1;
		}

		/* Initialise SDL image */
		if (IMG_Init(IMG_INIT_PNG) < 0) {
			std::cerr << "ERROR:" << SDL_GetError() << '\n';
			return 1;
		}

		if (win.create_window(1024, 512))
			return 1;

		ply.init("src/assets/player.png", &win.ren);
		event_loop(win, ply);
		return 0;
	}
}

int
main(void)
{
	return game::setup();
}
