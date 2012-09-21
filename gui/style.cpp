#include "style.h"
#include "draw/SDL_draw.h"
#include "SDL_image.h"
#include <iostream>
#include <cstring>

Style* Style::_inst = NULL;

int Style::_wheel_sizes[WHEEL_SIZES] = { 10, 16, 24, 32, 48, 64 };

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

/*	_shade.r = 255;
	_shade.g = 255;
	_shade.b = 255;*/

	_medium.r = 128;
	_medium.g = 128;
	_medium.b = 192;

	_light.r = 192;
	_light.g = 192;
	_light.b = 255;
}

void Style::loadImages() {
        for (int i = 0; i < WHEEL_SIZES; ++i) {
          char buffer[32];
          sprintf(buffer, "data/images/knob%i.png", _wheel_sizes[i]);

          _wheel_images[i] = IMG_Load(buffer);
	  if (!_wheel_images[i]) {
	    std::cerr << "Could not load image " << buffer << std::endl;
          }
        }
}

void Style::unloadImages() {
        for (int i = 0; i < WHEEL_SIZES; ++i) {
          SDL_FreeSurface(_wheel_images[i]);
        }
}

void Style::drawInset(SDL_Surface* surf, int x, int y, int w, int h, int depth) {
	Uint32 sh = SDL_MapRGB(surf->format, _shade.r, _shade.g, _shade.b);
	Uint32 li = SDL_MapRGB(surf->format, _light.r, _light.g, _light.b);

	Draw_FillRect(surf, x, y, w, depth, sh);
	Draw_FillRect(surf, x, y+h-depth, w, depth, li);
	Draw_FillRect(surf, x, y, depth, h, sh);
	Draw_FillRect(surf, x+w-depth, y, depth, h, li);
}

void Style::drawOutset(SDL_Surface* surf, int x, int y, int w, int h, int depth) {
	Uint32 sh = SDL_MapRGB(surf->format, _shade.r, _shade.g, _shade.b);
	Uint32 li = SDL_MapRGB(surf->format, _light.r, _light.g, _light.b);

	Draw_FillRect(surf, x, y, w, depth, li);
	Draw_FillRect(surf, x, y+h-depth, w, depth, sh);
	Draw_FillRect(surf, x, y, depth, h, li);
	Draw_FillRect(surf, x+w-depth, y, depth, h, sh);
}

int Style::drawWheel(SDL_Surface* surf, int x, int y, int w, int h) {
  int smaller = w < h ? w : h;
  if (smaller < 10) return 0;

  // Find first size larger than smaller dimension of the wheel
  int i;
  for (i = 0; i < WHEEL_SIZES; ++i) {
    if (_wheel_sizes[i] > smaller) {
      break;
    }
  }

  // Use the previous size
  int size = _wheel_sizes[i-1];

  SDL_Rect src;
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  src.x = 0;
  src.y = 0;
  src.w = size;
  src.h = size;
  SDL_BlitSurface(_wheel_images[i-1], &src, surf, &dst);

  return size;
}

