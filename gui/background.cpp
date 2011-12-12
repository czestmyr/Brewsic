#include "background.h"
#include "style.h"

void Background::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Color bgcolor = Style::inst()->getBgColor();
	Uint32 bg = SDL_MapRGB(surf->format, bgcolor.r, bgcolor.g, bgcolor.b);
	SDL_Color shadecol = Style::inst()->getShadeColor();
	Uint32 shade = SDL_MapRGB(surf->format, shadecol.r, shadecol.g, shadecol.b);

	SDL_FillRect(surf, &_rect, bg);

	//Corners
	SDL_Rect crect;
	crect.w = 10;
	crect.h = 10;

	crect.x = _x; crect.y = _y;
	SDL_FillRect(surf, &crect, shade);
	crect.x = _x + _w - 10; crect.y = _y;
	SDL_FillRect(surf, &crect, shade);
	crect.x = _x + _w - 10; crect.y = _y + _h - 10;
	SDL_FillRect(surf, &crect, shade);
	crect.x = _x; crect.y = _y + _h - 10;
	SDL_FillRect(surf, &crect, shade);

	Draw_FillCircle(surf, _x+10, _y+10, 10, bg);
	Draw_FillCircle(surf, _x+_w-10, _y+10, 10, bg);
	Draw_FillCircle(surf, _x+_w-10, _y+_h-10, 10, bg);
	Draw_FillCircle(surf, _x+10, _y+_h-10, 10, bg);
}

