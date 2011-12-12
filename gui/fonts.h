#ifndef _FONTS_H_
#define _FONTS_H_

#include <SDL.h>
#include "SDL_ttf.h"

class Fonts {
	public:
		Fonts();

		static Fonts* inst() {
			if (_inst == NULL) {
				_inst = new Fonts();
			}
			return _inst;
		}

		void renderText(const char* text, SDL_Surface* surf, int x, int y, SDL_Color color);
		SDL_Surface* getRenderedText(const char* text, SDL_Surface* surf, SDL_Color color);
		int getTextWidth(const char* text);
	private:
		static Fonts* _inst;

		TTF_Font* _16pt;
};

#endif

