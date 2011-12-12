#include "adsr.h"
#include <SDL.h>

#define SPEED 0.0001

void Adsr::filter(int bufsize, float* buffer) {
	float coeff;
	for (int i = 0; i < bufsize; ++i) {
		_phase += _phase_inc;
		if (_phase >= _r) {
			coeff = 0.0;
		} else if (_phase < _a) {
			coeff = _phase / _a;
		} else if (_phase < _d) {
			coeff = 1 - 0.4 * (_phase - _a) / (_d - _a);
		} else if (_phase < _s) {
			coeff = 0.6;
		} else if (_hold) {
			_phase = _s;
			coeff = 0.6;
		} else {
			coeff = 0.6 - 0.6 * (_phase - _s) / (_r - _s);
		}

		if (_curr > coeff) {
			if (_curr > coeff + SPEED) {
				_curr -= SPEED;
			} else {
				_curr = coeff;
			}
		} else if (_curr < coeff) {
			if (_curr < coeff - SPEED) {
				_curr += SPEED;
			} else {
				_curr = coeff;
			}
		}

		buffer[i] *= _curr;
	}
}

void Adsr::trigger() {
	SDL_LockAudio();
	_hold = true;
	_phase = 0.0;
	SDL_UnlockAudio();
}

void Adsr::release() {
	SDL_LockAudio();
	_hold = false;
	SDL_UnlockAudio();
}

