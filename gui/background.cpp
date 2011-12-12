#include "background.h"

void Background::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_FillRect(surf, &_rect, SDL_MapRGB(surf->format, 20, 25, 25));
}

