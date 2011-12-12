#ifndef _FILTER_VOLUME_H_
#define _FILTER_VOLUME_H_

#include "Ifilter.h"

class Volume: public IFilter {
	public:
		Volume(float vol): _vol(vol) {}

		~Volume() {};

		void filter(int bufsize, float* buffer) {
			for (int i = 0; i < bufsize; ++i) {
				buffer[i] *= _vol;
			}
		}
	private:
		float _vol;
};

#endif

