#include "flanger.h"
#include <cmath>

inline float lerp_array(int idx_a, int idx_b, float t, float* arr) {
	return arr[idx_a]*(1.0-t) + arr[idx_b]*t;
}

void FlangerFilter::filter(int bufsize, float* buffer) {
	double dummy;
	// Calculate what the offset will be after filtering this buffer
	float new_offset = modf((_offset + _inc * bufsize) / _size, &dummy) * _size;

	// Proceed from the end of the buffer to the beginning
	for (int i = bufsize-1; i >= 0; --i) {
		_offset -= _inc;

		// If the offset underflows, start from the end again
		if (_offset < 0)
			_offset += _size;

		// Take indices of the two buffer cells to be interpolated and
		// calculate the interpolation coefficient
		int idx_a = (int)floor(i - _offset);
		int idx_b =  (int)ceil(i - _offset);
		float t = (i-_offset)-idx_a;

		// Now interpolate either from the buffer itself or from the leftover
		// of the previously filtered data (i.e. temporary buffer)
		if (idx_a < -1) {
			buffer[i] = _wet*lerp_array(idx_a+_size, idx_b+_size, t, _b) + _dry*buffer[i];
		} else if (idx_a == 0) {
			buffer[i] = _wet*(_b[_size-1]*(1.0-t) + buffer[0]*t) + _dry*buffer[i];
		} else {
			buffer[i] = _wet*lerp_array(idx_a, idx_b, t, buffer) + _dry*buffer[i];
		}
	}

	//Copy part of the old memory into temporary buffer
	memcpy(_b, &buffer[bufsize - _size], sizeof(float));
	_offset = new_offset;
}

