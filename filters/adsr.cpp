#include "adsr.h"
#include <SDL.h>

#include "gui/wheel.h"
#include "gui/frame.h"
#include "gui/label.h"

#define SPEED 0.0001

void Adsr::filter(int bufsize, float* buffer) {
	_a = _attack;
	_d = _a + _decay;
	_s = _d + _sustain;
	_r = _s + _release;

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

		buffer[i] *= coeff;
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

void Adsr::prepareGui() {
	_gui = safe_new(Frame((IControl*)NULL, 0, 0, 0, 0, 1));

        SafePtr<IControl> frame1 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
        frame1->packHorizontally(2);
        SafePtr<IControl> label1 = safe_new(Label(frame1, 0, 0, "A"));
        label1->setPreferedSize(0, 0, 1);
	safe_new(Wheel(frame1, 0, 0, 0, 0, 10, 10000, &_attack))->setPreferedSize(32, 32, 1);
        SafePtr<IControl> frame2 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
        frame2->packHorizontally(2);
        SafePtr<IControl> label2 = safe_new(Label(frame2, 0, 0, "D"));
        label2->setPreferedSize(0, 0, 1);
	safe_new(Wheel(frame2, 0, 0, 0, 0, 0, 10000, &_decay))->setPreferedSize(32, 32, 1);
        SafePtr<IControl> frame3 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
        frame3->packHorizontally(2);
        SafePtr<IControl> label3 = safe_new(Label(frame3, 0, 0, "S"));
        label3->setPreferedSize(0, 0, 1);
	safe_new(Wheel(frame3, 0, 0, 0, 0, 0, 10000, &_sustain))->setPreferedSize(32, 32, 1);
        SafePtr<IControl> frame4 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
        frame4->packHorizontally(2);
        SafePtr<IControl> label4 = safe_new(Label(frame4, 0, 0, "R"));
        label4->setPreferedSize(0, 0, 1);
	safe_new(Wheel(frame4, 0, 0, 0, 0, 0, 10000, &_release))->setPreferedSize(32, 32, 1);
	_gui->packHorizontally(5);
        _gui->setPreferedSize(0, 36, 1);
}

