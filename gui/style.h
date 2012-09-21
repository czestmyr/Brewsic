#ifndef _STYLE_H_
#define _STYLE_H_

#include <SDL.h>

class Style {
	public:
		Style();

		static Style* inst() {
			if (_inst == NULL) {
				_inst = new Style();
			}
			return _inst;
		}

		SDL_Color getBgColor() { return _bgcolor; }
		SDL_Color getFgColor() { return _fgcolor; }
		SDL_Color getShadeColor() { return _shade; }
		SDL_Color getMediumColor() { return _medium; }
		SDL_Color getLightColor() { return _light; }

                void loadImages();
                void unloadImages();

		void drawInset(SDL_Surface* surf, int x, int y, int w, int h, int depth);
		void drawOutset(SDL_Surface* surf, int x, int y, int w, int h, int depth);
                /// Returns the diameter of the wheel used
                int drawWheel(SDL_Surface* surf, int x, int y, int w, int h);
	private:
		static Style* _inst;

		SDL_Color _bgcolor;
		SDL_Color _fgcolor;
		SDL_Color _shade;
		SDL_Color _medium;
		SDL_Color _light;

                #define WHEEL_SIZES 6
                SDL_Surface* _wheel_images[WHEEL_SIZES];
                static int _wheel_sizes[WHEEL_SIZES];
};

#endif

