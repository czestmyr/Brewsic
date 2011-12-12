#include "fonts.h"
#include <iostream>

#define MAIN_FONT "data/dalila_light.ttf"

Fonts* Fonts::_inst = NULL;

Fonts::Fonts() {
	TTF_Init();

	_16pt = TTF_OpenFont(MAIN_FONT, 16);
	if (!_16pt) {
		std::cerr << "Could not open font file " << MAIN_FONT << ": " << TTF_GetError() << std::endl;
	}
}

void Fonts::renderText(const char* text, SDL_Surface* surf, int x, int y, SDL_Color color) {
	SDL_Surface* textSurf = TTF_RenderText_Blended(_16pt, text, color);
	if (!textSurf) {
		std::cerr << "Could not render text: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_BlitSurface(textSurf, NULL, surf, &dst);
	SDL_FreeSurface(textSurf);
}
