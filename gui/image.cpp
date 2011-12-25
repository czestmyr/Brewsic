#include "image.h"
#include "SDL_image.h" 
#include <iostream>

Image::Image(SafePtr<IControl> parent, int x, int y, int w, int h, const char* filename)
: IControl(parent) {
	redim(x, y, w, h);
	_size.x = 0;
	_size.y = 0;
	_size.w = _w;
	_size.h = _h;

	_img = IMG_Load(filename);
	if (!_img) {
		std::cerr << "Could not load image " << filename << std::endl;
	}
}

Image::~Image() {
	if (_img)
		SDL_FreeSurface(_img);
}

void Image::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Rect dst;
	dst.x = orig_x + _x;
	dst.y = orig_y + _y;
	SDL_BlitSurface(_img, &_size, surf, &dst);
}

