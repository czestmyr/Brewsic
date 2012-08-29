#include "pselect.h"
#include "style.h"
#include "SDL_image.h"
#include <iostream>

PictureSelector::PictureSelector(SafePtr<IControl> parent, int x, int y, int w, int h, Property<int>* prop)
: IControl(parent), _prop(prop) {
	setPreferedSize(32, 32, 1);
	redim(x, y, w, h);
}

PictureSelector::~PictureSelector() {
	for (int i = 0; i < _pics.size(); ++i) {
		SDL_FreeSurface(_pics[i]);
	}
}

void PictureSelector::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	if (_pics.size() <= *_prop) return;
	SDL_Rect dst;
	dst.x = orig_x + _x;
	dst.y = orig_y + _y;
	_size.x = 0;
	_size.y = 0;
	_size.w = _w;
	_size.h = _h;
	SDL_BlitSurface(_pics[*_prop], &_size, surf, &dst);
	Style::inst()->drawInset(surf, orig_x + _x, orig_y + _y, _w, _h, 2);
}

bool PictureSelector::leftPress(int x, int y) {
	if (_pics.size() == 0 || _prop == NULL) return false;
	*_prop = (*_prop + 1) % _pics.size();
}

void PictureSelector::addPicture(const char* filename) {
	SDL_Surface* newPic = IMG_Load(filename);
	if (!newPic) {
		std::cerr << "Could not load image " << filename << std::endl;
		return;
	}
	_pics.push_back(newPic);
}

