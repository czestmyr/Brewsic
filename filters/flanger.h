#ifndef _FLANGER_FILTER_H_
#define _FLANGER_FILTER_H_

#include "Ifilter.h"
#include <cstring>

class FlangerFilter: public IFilter {
	public:
		FlangerFilter(int size) {
			_b = new float[size];
			memset(_b, 0, _size*sizeof(float));
			_offset = 0.0;
			_inc = 0.00001;
			_size = size;
			_dry = 0.95;
			_wet = 0.05;
		}

		~FlangerFilter() {
			delete[] _b;
		}

		void filter(int bufsize, float* buffer);
	private:
		float* _b;
		float _offset;
		float _inc;
		int _size;

		float _dry;
		float _wet;
};

#endif

