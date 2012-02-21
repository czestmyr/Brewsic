#include "gui/frame.h"
#include "style.h"

void Frame::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Style::inst()->drawInset(surf, orig_x+_x+1, orig_y+_y+1, _w-2, _h-2, 1);
}

