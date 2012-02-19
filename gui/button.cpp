#include "button.h"
#include "style.h"
#include "fonts.h"
#include "common/property.h"

Button::Button(SafePtr<IControl> parent, int x, int y, const char* text, Signal sig)
: IControl(parent), _text(text), _sig(sig) {
	int w = Fonts::inst()->getTextWidth(text) + 8;
	redim(x, y, w, 20);
	_textSurf = NULL;
	_pressed = false;
}

Button::~Button() {
	if (_textSurf)
		SDL_FreeSurface(_textSurf);
}

void Button::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Uint32 light = SDL_MapRGB(surf->format, 192, 192, 192);
	Uint32 dark = SDL_MapRGB(surf->format, 64, 64, 100);
	Uint32 darkest = SDL_MapRGB(surf->format, 32, 32, 50);

	SDL_Rect dst;
	int x = orig_x + _x;
	int y = orig_y + _y;

	if (_pressed) {
		dst.x = x; dst.y = y; dst.w = _w; dst.h = _h;
		SDL_FillRect(surf, &dst, darkest);
		Style::inst()->drawInset(surf, x, y, _w, _h, 2);
	} else {
		dst.x = x; dst.y = y; dst.w = _w; dst.h = _h;
		SDL_FillRect(surf, &dst, darkest);
		Style::inst()->drawOutset(surf, x, y, _w, _h, 2);
	}

	if (!_textSurf) {
		SDL_Color textCol;
		textCol.r = textCol.g = textCol.b = 255;
		_textSurf = Fonts::inst()->getRenderedText(_text.c_str(), surf, textCol);
	}

	dst.x = x + (_pressed?4:5);
	dst.y = y + (_pressed?1:2);
	SDL_BlitSurface(_textSurf, NULL, surf, &dst);
}

bool Button::leftPress(int x, int y) {
	_pressed = true;

	return true;
}

bool Button::leftRelease(int x, int y) {
	if (inside(x,y) && _pressed) {
		_pressed = false;
		_sig();
	} else 
		_pressed = false;

	return true;
}

