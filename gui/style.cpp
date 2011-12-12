#include "style.h"

Style* Style::_inst = NULL;

Style::Style() {
	_bgcolor.r = 20;
	_bgcolor.g = 30;
	_bgcolor.b = 50;

	_fgcolor.r = 64;
	_fgcolor.g = 64;
	_fgcolor.b = 92;

	_shade.r = 0;
	_shade.g = 5;
	_shade.b = 10;

	_light.r = 192;
	_light.g = 192;
	_light.b = 255;
}

