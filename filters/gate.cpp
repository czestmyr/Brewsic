#include "gate.h"

void GateFilter::filter(int bufsize, float* buffer) {
	for (int i = 0; i < bufsize; ++i) {
		_phase += _phase_inc;
		while (_phase > 1.0) _phase -= 1.0;
		_active = _phase < _duty;
		if (_active) {
			_val -= _phase_inc*_speed;
		} else {
			_val += _phase_inc*_speed;
		}
		if (_val > 1.0) {
			_val = 1.0;
		} else if (_val < 0.0) {
			_val = 0.0;
		}
		buffer[i] *= _val;
	}
}

