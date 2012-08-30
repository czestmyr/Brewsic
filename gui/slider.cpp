#include "slider.h"
#include "style.h"
#include "fonts.h"
#include <cmath>

#define SLIDER_WIDTH         20

Slider::Slider(SafePtr<IControl> parent, int x, int y, int h, float min, float max, Property<float>* prop, int bh)
: IControl(parent), _prop(prop), _slider_button_height(bh) {
	if (h < _slider_button_height)
		h = _slider_button_height;

	if (min > max) {
		float swap = min;
		min = max;
		max = swap;
	}
	_min = min;
	_max = max;

        if (_prop) {
	  _prop->addObserver(this);
        }

	redim(x, y, SLIDER_WIDTH, h);
	setPreferedSize(SLIDER_WIDTH, 0, 1);

	_pressed = false;
}

Slider::~Slider() {
	if (_prop) {
		_prop->removeObserver(this);
	}
}

void Slider::redim(int x, int y, int w, int h) {
	IControl::redim(x, y, w, h);
	_inc = (_max - _min) / _h;

	if (_prop) {
		setValue(*_prop);
	} else {
		setValue(0.0);
	}
}

void Slider::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Style::inst()->drawInset(
		surf,
		orig_x + _x + _w/2 - 2,
		orig_y + _y,
		4,
		_h,
		2
	);

	SDL_Color bgCol = Style::inst()->getBgColor();
	Uint32 bgColU32 = SDL_MapRGB(surf->format, bgCol.r, bgCol.g, bgCol.b);
	Draw_FillRect(surf, orig_x + _x, orig_y + _y + _button_y, _w, _slider_button_height, bgColU32);

	SDL_Color textCol;
	textCol.r = textCol.g = textCol.b = 255.0;
	if (_pressed) {
		Style::inst()->drawInset(surf, orig_x + _x, orig_y + _y + _button_y, _w, _slider_button_height, 2);
		char buffer[32];
		sprintf(buffer, "%.1f", _value);
		Fonts::inst()->renderText(buffer, surf, orig_x + _x + _w, orig_y + _y + _button_y, textCol);
	} else {
		Style::inst()->drawOutset(surf, orig_x + _x, orig_y + _y + _button_y, _w, _slider_button_height, 2);
	}
}

bool Slider::leftPress(int x, int y) {
	if (y >= _button_y + _y && y <= _button_y + _y + _slider_button_height) {
		_pressed = true;
		_pressed_y = y;
		_pressed_value = _value;
		return true;
	}
}

bool Slider::leftRelease(int x, int y) {
	_pressed = false;
	return true;
}

bool Slider::mouseMove(int x, int y, int dx, int dy) {
	if (_pressed) {
		setValue(_pressed_value + (_pressed_y - y) * _inc);
		return true;
	}
	return false;
}

float Slider::getValue() {
	return _value;
}

void Slider::setValue(float val) {
	setValueInternal(val);
}

void Slider::signal() {
	setValueInternal(*_prop, true);
}

void Slider::disconnect() {
	_prop = NULL;
}

void Slider::setValueInternal(float val, bool bySignal) {
	if (val < _min) val = _min;
	if (val > _max) val = _max;
	_value = val;

	// Calculate the position of the slide button
	float t = (_max - _value) / (_max - _min);
	_button_y = t * (_h - _slider_button_height);

	if (!bySignal && _prop)
		*_prop = _value;
}

