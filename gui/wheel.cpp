#include "wheel.h"
#include "style.h"
#include "fonts.h"
#include <cmath>

Wheel::Wheel(IControl* parent, int x, int y, int w, int h, int min, int max, void* data)
: IControl(parent), _data(data), _callback(NULL) {
	redim(x, y, w, h);

	if (min > max) {
		int swap = min;
		min = max;
		max = swap;
	}
	_min = min;
	_max = max;
	_inc = (_max - _min) / 200.0;
	setValue(0);

	_r = w<h ? w/2 : h/2;
	_pressed = false;
}

void Wheel::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Color light = Style::inst()->getLightColor();
	SDL_Color fg = Style::inst()->getFgColor();
	SDL_Color shade = Style::inst()->getShadeColor();
	SDL_Color text; text.r = text.g = text.b = 255;

	Uint32 licol = SDL_MapRGB(surf->format, light.r, light.g, light.b);
	Uint32 fgcol = SDL_MapRGB(surf->format, fg.r, fg.g, fg.b);
	Uint32 shcol = SDL_MapRGB(surf->format, shade.r, shade.g, shade.b);

	Draw_FillCircle(surf, orig_x + _x + _r, orig_y + _y + _r, _r, licol);
	Draw_FillCircle(surf, orig_x + _x + _r, orig_y + _y + _r, _r-2, fgcol);

	// Calculate the angle at which the wheel should be right now
	float t = (float(_max) - float(_value)) / (float(_max) - float(_min));
	float delta = 0.3;                               // Some small value to differentiate min and max
	float phimin = 4.71238898 - delta + 6.283185307; // 4.71... = 3/2 * pi; 6.28... = 2 * pi
	float phimax = 4.71238898 + delta;               // 4.71... = 3/2 * pi
	float phi = t*phimin + (1.0-t)*phimax;

	int x = cos(phi) * (_r-3);
	int y = (-sin(phi)) * (_r-3);               // Y axis is inverted in SDL, thus 1.0 - sin(...)
	Draw_Line(surf, orig_x + _x + _r, orig_y + _y + _r, orig_x + _x + _r + x, orig_y + _y + _r + y, licol);
	Draw_FillCircle(surf, orig_x + _x + _r + x, orig_y + _y + _r + y, 3, shcol);

	if (_pressed) {
		char buffer[32];
		sprintf(buffer, "%i", _value);
		Fonts::inst()->renderText(buffer, surf, orig_x + _x + _r + x + 4, orig_y + _y + _r + y, text);
	}
}

bool Wheel::leftPress(int x, int y) {
	_pressed = true;
	_pressed_y = y;
	_pressed_value = _value;
	return true;
}

bool Wheel::leftRelease(int x, int y) {
	_pressed = false;
	return true;
}

bool Wheel::mouseMove(int x, int y, int dx, int dy) {
	if (_pressed) {
		setValue(_pressed_value + (_pressed_y - y) * _inc);
		return true;
	}
	return false;
}

void Wheel::setCallback(void (*callback)(void* data)) {
	_callback = callback;
}

int Wheel::getValue() {
	return _value;
}

void Wheel::setValue(int val) {
	if (val < _min) val = _min;
	if (val > _max) val = _max;
	_value = val;

	if (_callback)
		_callback(_data);
}

