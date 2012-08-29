#include "keyboard.h"
#include "style.h"
#include "fonts.h"
#include <cmath>
#include <cstdio>
#include <SDL_image.h>

#define KEY_H 16
#define KEY_W 80

enum keyTypes {
	KT_WHITE_WIDE,
	KT_WHITE_NARROW,
	KT_BLACK_NARROW
};

keyTypes octaveTypes[12] = {
	KT_WHITE_NARROW,
	KT_BLACK_NARROW,
	KT_WHITE_WIDE,
	KT_BLACK_NARROW,
	KT_WHITE_NARROW,
	KT_WHITE_NARROW,
	KT_BLACK_NARROW,
	KT_WHITE_WIDE,
	KT_BLACK_NARROW,
	KT_WHITE_WIDE,
	KT_BLACK_NARROW,
	KT_WHITE_NARROW
};

const char* noteNames[12] = {
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"
};

Keyboard::Keyboard(SafePtr<IControl> parent, int x, int y, int h)
: IControl(parent){
	redim(x, y, 80, h);
	_shift = 720;
	_synth = (ISynth*)NULL;
	_lastNote = -1;

	_kww = IMG_Load("data/images/kww.png");
	_kwn = IMG_Load("data/images/kwn.png");
	_kbn = IMG_Load("data/images/kbn.png");
}

void Keyboard::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Rect dst;
	dst.x = orig_x + _x;
	int y = orig_y + _y - _shift;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = KEY_W;
	src.h = 0;

	int y_begin = orig_y + _y;
	int y_end   = orig_y + _y + _h;

	int inc;
	char buffer[16];
	for (int o = 0; o <= 8; ++o) {            // Octave
		for (int k = 0; k < 12; ++k) {    // White Keys
			if (octaveTypes[k] == KT_BLACK_NARROW) continue;

			switch (octaveTypes[k]) {
				case KT_WHITE_WIDE:
					inc = 32;
				break;
				case KT_WHITE_NARROW:
					inc = 24;
				break;
			}

			if      (y >= y_begin)       { src.y = 0; dst.y = y; }
			else if (y >= y_begin - inc) { src.y = y_begin - y; dst.y = y_begin; }
			else                         { src.y = inc; dst.y = y; }
			if      (y + inc < y_end)    src.h = inc - src.y;
			else if (y < y_end)          src.h = y_end - y;
			else                         src.h = 0;

			switch (octaveTypes[k]) {
				case KT_WHITE_WIDE:
					SDL_BlitSurface(_kww, &src, surf, &dst);
					y += inc;
				break;
				case KT_WHITE_NARROW:
					SDL_BlitSurface(_kwn, &src, surf, &dst);
					y += inc;
				break;
			}
		}
		y -= 3*32 + 4*24;
		for (int k = 0; k < 12; ++k) {    // Black Keys
			if      (y >= y_begin)      { src.y = 0; dst.y = y; }
			else if (y >= y_begin - 16) { src.y = y_begin - y; dst.y = y_begin; }
			else                        { src.y = 16; dst.y = y; }
			if      (y + 16 < y_end) src.h = 16 - src.y;
			else if (y < y_end)      src.h = y_end - y;
			else                     src.h = 0;

			sprintf(buffer, "%s  %i", noteNames[k], o);
			switch (octaveTypes[k]) {
				case KT_BLACK_NARROW:
					SDL_BlitSurface(_kbn, &src, surf, &dst);
					if (y >= y_begin && y < y_end - 16)
						Fonts::inst()->renderText(buffer, surf, dst.x + 5, y, Style::inst()->getLightColor());
					y += 16;
				break;
				default:
					if (y >= y_begin && y < y_end - 16)
						Fonts::inst()->renderText(buffer, surf, dst.x + 5, y, Style::inst()->getShadeColor());
					y += 16;
			}
		}
	}
}

bool Keyboard::leftPress(int x, int y) {
	int note = (y - _y + _shift) / 16;
	if (note < 0 || note >= 12*8) return false;

	if (_synth) _synth->startNote(note, 440.0*pow(2.0,float(note-57)/12.0));
	_lastNote = note;
	return true;
}

bool Keyboard::leftRelease(int x, int y) {
	if (_synth) _synth->stopNote(_lastNote);
	_lastNote = -1;
	if (inside(x,y)) return true;
}

bool Keyboard::keyPress(SDLKey sym) {
	int note = keySymToNote(sym);
	if (note < 0 || note >= 12*8) return false;

	if (_synth) _synth->startNote(note, 440.0*pow(2.0,float(note-57)/12.0));
	return true;
}

bool Keyboard::keyRelease(SDLKey sym) {
	int note = keySymToNote(sym);
	if (note < 0 || note >= 12*8) return false;

	if (_synth) _synth->stopNote(note);
	return true;
}

int Keyboard::keySymToNote(SDLKey sym) {
	switch (sym) {
		case SDLK_z: return 46;
		case SDLK_s: return 47;
		case SDLK_x: return 48;
		case SDLK_d: return 49;
		case SDLK_c: return 50;
		case SDLK_v: return 51;
		case SDLK_g: return 52;
		case SDLK_b: return 53;
		case SDLK_h: return 54;
		case SDLK_n: return 55;
		case SDLK_j: return 56;
		case SDLK_m: return 57;
	}

	return -1;
}

