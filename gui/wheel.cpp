#include "wheel.h"
#include "style.h"
#include "fonts.h"
#include <cmath>

Wheel::Wheel(SafePtr<IControl> parent, int x, int y, int w, int h, float min, float max, Property<float>* prop)
: IControl(parent), PropertyObserver<float>(prop) {
	redim(x, y, w, h);

	if (min > max) {
		float swap = min;
		min = max;
		max = swap;
	}
	_min = min;
	_max = max;
	_inc = (_max - _min) / 200.0;

	setValueInternal(getProp(), true);

	_pressed = false;
}

Wheel::~Wheel() {}

void Wheel::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Color light = Style::inst()->getLightColor();
	SDL_Color fg = Style::inst()->getFgColor();
	SDL_Color shade = Style::inst()->getShadeColor();
	SDL_Color text; text.r = text.g = text.b = 255;

	Uint32 licol = SDL_MapRGB(surf->format, light.r, light.g, light.b);
	Uint32 fgcol = SDL_MapRGB(surf->format, fg.r, fg.g, fg.b);
	Uint32 shcol = SDL_MapRGB(surf->format, shade.r, shade.g, shade.b);

        int r = Style::inst()->drawWheel(surf, orig_x + _x, orig_y + _y, _w, _h) / 2;

	// Calculate the angle at which the wheel should be right now
	float t = (_max - _value) / (_max - _min);
	float delta = 0.3;                               // Some small value to differentiate min and max
	float phimin = 4.71238898 - delta + 6.283185307; // 4.71... = 3/2 * pi; 6.28... = 2 * pi
	float phimax = 4.71238898 + delta;               // 4.71... = 3/2 * pi
	float phi = t*phimin + (1.0-t)*phimax;

        int inner_r = (int)(r*0.7);
	int x = cos(phi) * (inner_r);
	int y = (-sin(phi)) * (inner_r);               // Y axis is inverted in SDL, thus 1.0 - sin(...)
	Draw_Line(surf, orig_x + _x + r, orig_y + _y + r, orig_x + _x + r + x, orig_y + _y + r + y, shcol);
	Draw_FillCircle(surf, orig_x + _x + r + x, orig_y + _y + r + y, 3, shcol);
	Draw_FillCircle(surf, orig_x + _x + r + x, orig_y + _y + r + y, 2, licol);

	if (_pressed) {
		char buffer[32];
		sprintf(buffer, "%.1f", _value);
		Fonts::inst()->renderText(buffer, surf, orig_x + _x + r + x + 4, orig_y + _y + r + y, text);
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

float Wheel::getValue() {
	return _value;
}

void Wheel::setValue(float val) {
	setValueInternal(val);
}

void Wheel::propertyChanged() {
	setValueInternal(getProp(), true);
}

void Wheel::setValueInternal(float val, bool byAction) {
	if (val < _min) val = _min;
	if (val > _max) val = _max;
	_value = val;

	if (!byAction && propValid())
		setProp(_value);
}

