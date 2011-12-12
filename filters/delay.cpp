#include "delay.h"

void DelayFilter::filter(int bufsize, float* buffer) {
	for (int i = 0; i < bufsize; ++i) {
		_previdx = _loopidx;
		_loopidx = (_loopidx+1)%_maxidx;
		buffer[i] = buffer[i] + 0.5*_loop[_loopidx] - 0.5*buffer[i]*_loop[_loopidx];
		_loop[_previdx] = buffer[i] * _atten;
	}
}

