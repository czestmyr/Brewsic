#include "button.h"
#include "fonts.h"

Button::Button(IControl* parent, int x, int y, const char* text, void* data)
: IControl(parent), _text(text), _data(data), _callback(NULL) {
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
	if (_pressed) {
		dst.x = orig_x + _x + 2; dst.y = orig_y + _y + 2; dst.w = _w - 4; dst.h = _h - 4;
		SDL_FillRect(surf, &dst, darkest);
		Draw_VLine(surf, orig_x + _x         , orig_y + _y         , orig_y + _y + _h - 2, dark);
		Draw_VLine(surf, orig_x + _x + _w - 1, orig_y + _y + 1     , orig_y + _y + _h - 2, dark);
		Draw_HLine(surf, orig_x + _x         , orig_y + _y         , orig_x + _x + _w - 2, dark);
		Draw_HLine(surf, orig_x + _x + 1     , orig_y + _y + _h - 1, orig_x + _x + _w - 2, dark);
		Draw_Rect (surf, orig_x + _x + 1     , orig_y + _y + 1     , _w-2, _h-2, light);
	} else {
		dst.x = orig_x + _x + 3; dst.y = orig_y + _y + 3; dst.w = _w - 4; dst.h = _h - 4;
		SDL_FillRect(surf, &dst, darkest);
		Draw_VLine(surf, orig_x + _x    , orig_y + _y    , orig_y + _y + _h - 2, dark);
		Draw_VLine(surf, orig_x + _x + 1, orig_y + _y + 1, orig_y + _y + _h - 1, dark);
		Draw_HLine(surf, orig_x + _x    , orig_y + _y    , orig_x + _x + _w - 2, dark);
		Draw_HLine(surf, orig_x + _x + 1, orig_y + _y + 1, orig_x + _x + _w - 1, dark);
		Draw_Rect (surf, orig_x + _x + 2, orig_y + _y + 2, _w-2, _h-2, light);
	}

	if (!_textSurf) {
		SDL_Color textCol;
		textCol.r = textCol.g = textCol.b = 255;
		_textSurf = Fonts::inst()->getRenderedText(_text.c_str(), surf, textCol);
	}

	dst.x = orig_x + _x + (_pressed?5:6);
	dst.y = orig_y + _y + (_pressed?2:3);
	SDL_BlitSurface(_textSurf, NULL, surf, &dst);
}

bool Button::leftPress(int x, int y) {
	_pressed = true;
}

bool Button::leftRelease(int x, int y) {
	if (inside(x,y) && _pressed) {
		if (_callback) {
			_callback(_data);
		}
	}
	_pressed = false;
}

