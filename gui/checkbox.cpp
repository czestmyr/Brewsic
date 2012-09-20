#include "checkbox.h"
#include "style.h"

Checkbox::Checkbox(SafePtr<IControl> parent, int x, int y, int w, int h, Property<bool>* prop)
: IControl(parent), PropertyObserver<bool>(prop) {
	redim(x, y, w, h);
	_pressed = false;
	setValueInternal(getProp(), true);
}

Checkbox::~Checkbox() {}

void Checkbox::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Color bg = Style::inst()->getBgColor();
	SDL_Color fg = Style::inst()->getFgColor();
	Uint32 bg32 = SDL_MapRGB(surf->format, bg.r, bg.g, bg.b);
	Uint32 fg32 = SDL_MapRGB(surf->format, fg.r, fg.g, fg.b);

	if (_pressed) {
		Draw_FillRect(surf, orig_x + _x, orig_y + _y, _w, _h, fg32);
	} else {
		Draw_FillRect(surf, orig_x + _x, orig_y + _y, _w, _h, bg32);
	}

	if (_value) {
		int r = _w>_h ? _w/4 : _h/4;
		Draw_FillCircle(surf, orig_x + _x + _w/2, orig_y + _y + _h/2, r, fg32);
		Style::inst()->drawInset(surf, orig_x + _x, orig_y + _y, _w, _h, 2);
	} else {
		Style::inst()->drawOutset(surf, orig_x + _x, orig_y + _y, _w, _h, 2);
	}
}

bool Checkbox::leftPress(int x, int y) {
	_pressed = true;
	return true;
}

bool Checkbox::leftRelease(int x, int y) {
	if (_pressed && inside(x,y)) {
		setValueInternal(!_value);
	}

	_pressed = false;
	return true;
}

void Checkbox::propertyChanged() {
        setValueInternal(getProp(), true);
}

void Checkbox::setValueInternal(bool value, bool byAction) {
	_value = value;

	if (!byAction && propValid()) {
		setProp(_value);
	}
}

