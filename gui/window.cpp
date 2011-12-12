#include "window.h"
#include "../draw/SDL_draw.h"

Window::Window(IControl parent, int x, int y, int w, int h)
: IControl(parent) {
	redim(x, y, w, h);
}

void Window::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Uint32 color = SDL_MapRGB(surf->format, 192, 192, 192);

	Draw_Rect(surf, orig_x + _x, orig_y + _y, _w, _h, color);
}

