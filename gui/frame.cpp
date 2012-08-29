#include "gui/frame.h"
#include "style.h"

void Frame::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	if (_thickness)	Style::inst()->drawInset(surf, orig_x+_x, orig_y+_y, _w, _h, _thickness);
}

