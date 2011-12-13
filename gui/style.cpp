#include "style.h"

Style* Style::_inst = NULL;

Style::Style() {
	_bgcolor.r = 20;
	_bgcolor.g = 30;
	_bgcolor.b = 50;

	_fgcolor.r = 64;
	_fgcolor.g = 64;
	_fgcolor.b = 92;

	_shade.r = 0;
	_shade.g = 5;
	_shade.b = 10;

	_light.r = 192;
	_light.g = 192;
	_light.b = 255;
}

void Style::drawInset(SDL_Surface* surf, int x, int y, int w, int h, int depth) {
	Uint32 sh = SDL_MapRGB(surf->format, _shade.r, _shade.g, _shade.b);
	Uint32 li = SDL_MapRGB(surf->format, _light.r, _light.g, _light.b);

	Draw_FillRect(surf, x, y, w, depth, sh);
	Draw_FillRect(surf, x, y+h-depth, w, depth, li);
	Draw_FillRect(surf, x, y, depth, h, sh);
	Draw_FillRect(surf, x+w-depth, y, depth, h, li);
}

void Style::drawOutset(SDL_Surface* surf, int x, int y, int w, int h, int depth) {
	Uint32 sh = SDL_MapRGB(surf->format, _shade.r, _shade.g, _shade.b);
	Uint32 li = SDL_MapRGB(surf->format, _light.r, _light.g, _light.b);

	Draw_FillRect(surf, x, y, w, depth, li);
	Draw_FillRect(surf, x, y+h-depth, w, depth, sh);
	Draw_FillRect(surf, x, y, depth, h, li);
	Draw_FillRect(surf, x+w-depth, y, depth, h, sh);
}

