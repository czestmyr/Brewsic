#include "background.h"

void Background::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Uint32 bgcolor = SDL_MapRGB(surf->format, 20, 25, 25);

	SDL_FillRect(surf, &_rect, bgcolor);

	//Corners
	SDL_Rect crect;
	crect.w = 10;
	crect.h = 10;

	crect.x = _x; crect.y = _y;
	SDL_FillRect(surf, &crect, SDL_MapRGB(surf->format, 0, 0, 0));
	crect.x = _x + _w - 10; crect.y = _y;
	SDL_FillRect(surf, &crect, SDL_MapRGB(surf->format, 0, 0, 0));
	crect.x = _x + _w - 10; crect.y = _y + _h - 10;
	SDL_FillRect(surf, &crect, SDL_MapRGB(surf->format, 0, 0, 0));
	crect.x = _x; crect.y = _y + _h - 10;
	SDL_FillRect(surf, &crect, SDL_MapRGB(surf->format, 0, 0, 0));

	Draw_FillCircle(surf, _x+10, _y+10, 10, bgcolor);
	Draw_FillCircle(surf, _x+_w-10, _y+10, 10, bgcolor);
	Draw_FillCircle(surf, _x+_w-10, _y+_h-10, 10, bgcolor);
	Draw_FillCircle(surf, _x+10, _y+_h-10, 10, bgcolor);
}

