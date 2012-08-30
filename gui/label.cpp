#include "label.h"
#include "fonts.h"
#include "../common/property.h"
#include <cstring>

using namespace std;

#define MARGIN 5

Label::Label(SafePtr<IControl> parent, int x, int y, Property<std::string>* prop)
: IControl(parent), _prop(prop), _text(NULL) {
	_textSurf = NULL;

	redim(x, y, 10, 10);

	if (prop) {
		_prop->addObserver(this);
	}
}

Label::Label(SafePtr<IControl> parent, int x, int y, const char* text)
: IControl(parent), _prop(NULL) {
	_textSurf = NULL;
        _text = new char[strlen(text)+1];
        strcpy(_text, text);

	redim(x, y, 10, 10);
}

Label::~Label() {
	if (_textSurf)
		SDL_FreeSurface(_textSurf);
  if (_text)
    delete[] _text;
}

void Label::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	Uint32 light = SDL_MapRGB(surf->format, 192, 192, 192);

        prepareSurface(surf);

	SDL_Rect dst; dst.x = _x + orig_x + MARGIN; dst.y = _y + orig_y;
	SDL_BlitSurface(_textSurf, NULL, surf, &dst);
}

void Label::signal() {
	if (_textSurf) SDL_FreeSurface(_textSurf);
	_textSurf = NULL;
}

void Label::disconnect() {
	_prop = NULL;
}

void Label::prepareSurface(SDL_Surface* surf) {
	if (!_textSurf) {
		SDL_Color textCol;
		textCol.r = textCol.g = textCol.b = 255;
		if (_prop) {
                  _const_text = (*_prop)->c_str();
		  _textSurf = Fonts::inst()->getRenderedText(_const_text, surf, textCol);
                } else {
		  _textSurf = Fonts::inst()->getRenderedText(_text, surf, textCol);
                }
	} 
}

