#include "mixer.h"
#include <cstring>

void Mixer::clear() {
	memset(_buffer, 0, sizeof(float)*_bufsize);
}

void Mixer::mixIn(float* buffer) {
	for (int i = 0; i < _bufsize; ++i) {
		_buffer[i] = _buffer[i] + buffer[i];
	}
}

void Mixer::copyBuffer(Uint16* buffer) {
	for (int i = 0; i < _bufsize; ++i) {
		buffer[i] = _buffer[i] * 30000;
	}
}

void Mixer::copyBufferFloat(float* buffer) {
	memcpy(buffer, _buffer, sizeof(float)*_bufsize);
}

