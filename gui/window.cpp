#include "window.h"
#include "fonts.h"

Window::Window(IControl* parent, int x, int y, int w, int h)
: IControl(parent) {
	redim(x, y, w, h);
	_status_h = 20;
	_title_h = 20;
	_name = "Unnamed Window";
	_dragging = false;
}

void Window::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Uint32 color = SDL_MapRGB(surf->format, 192, 192, 192);

	SDL_Rect rect;
	rect.x = orig_x + _x;
	rect.y = orig_y + _y;
	rect.w = _w;
	rect.h = _h;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, 0, 0, 0));

	Draw_Rect(surf, orig_x + _x, orig_y + _y, _w, _h, color);
	Draw_Rect(surf, orig_x + _x, orig_y + _y, _w, _title_h, color);
	Draw_Rect(surf, orig_x + _x, orig_y + _y + _h - _status_h, _w, _status_h, color);

	// Title
	SDL_Color white;
	white.r = white.g = white.b = 255;
	Fonts::inst()->renderText(_name.c_str(), surf, orig_x + _x + 3, orig_y + _y + 2, white);

	// Status
	if (!_status.empty()) {
		SDL_Color grey;
		grey.r = grey.g = grey.b = 128;
		Fonts::inst()->renderText(_status.c_str(), surf, orig_x + _x + 3, orig_y + _y + _h - _status_h + 2, grey);
	}
}

bool Window::leftPress(int x, int y) {
	if (y < _y + _title_h) {
		_dragging = true;
		_status = "dragging...";
	}
	return true;
}

bool Window::leftRelease(int x, int y) {
	_dragging = false;
	_status = "";
	return true;
}

bool Window::mouseMove(int x, int y, int dx, int dy) {
	if (_parent && _dragging) {
		redim(_x+dx, _y+dy, _w, _h);
	}

	return true;
}

