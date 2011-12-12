#ifndef _FILTER_H_
#define _FILTER_H_

class IFilter {
	public:
		IFilter() {
			_samp_freq = 22050;
		}

		virtual ~IFilter() {};
		virtual void filter(int bufsize, float* buffer) = 0;

	protected:
		float _samp_freq;
};

#endif

