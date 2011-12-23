#ifndef _DELAY_FILTER_H_
#define _DELAY_FILTER_H_

#include "Ifilter.h"
#include "common/property.h"
#include <cstring>

class DelayFilter: public IFilter {
	public:
		DelayFilter(int len, float attenuation = 0.8) {
			_atten = attenuation;

			_loop = new float[len];
			memset(_loop, 0, sizeof(float)*len);
			_loopidx = 0;
			_maxidx = len;
			_previdx = len-1;
		};

		~DelayFilter() {
			delete[] _loop;
		};

		void filter(int bufsize, float* buffer);

		Property<float> _atten;

	private:
		float* _loop;
		int _loopidx;
		int _maxidx;
		int _previdx;
};

#endif

