#include "pselect.h"
#include "SDL_image.h"
#include <iostream>

PictureSelector::PictureSelector(IControl* parent, int x, int y, int w, int h, void* data)
: IControl(parent) {
	redim(x, y, w, h);
	_size.x = 0;
	_size.y = 0;
	_size.w = _w;
	_size.h = _h;
	_callback = NULL;
	_data = data;
	_selection = 0;
}

PictureSelector::~PictureSelector() {
	for (int i = 0; i < _pics.size(); ++i) {
		SDL_FreeSurface(_pics[i]);
	}
}

void PictureSelector::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	if (_pics.size() == 0) return;
	SDL_Rect dst;
	dst.x = orig_x + _x;
	dst.y = orig_y + _y;
	SDL_BlitSurface(_pics[_selection], &_size, surf, &dst);
}

bool PictureSelector::leftPress(int x, int y) {
	if (_pics.size() == 0) return false;
	_selection = (_selection + 1) % _pics.size();
	if (_callback)
		_callback(_data);
}

void PictureSelector::addPicture(const char* filename) {
	SDL_Surface* newPic = IMG_Load(filename);
	if (!newPic) {
		std::cerr << "Could not load image " << filename << std::endl;
		return;
	}
	_pics.push_back(newPic);
}

void PictureSelector::setCallback(void (*callback)(void* data)) {
	_callback = callback;
}

