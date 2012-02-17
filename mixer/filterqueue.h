#ifndef _FILTER_QUEUE_H_
#define _FILTER_QUEUE_H_

#include <vector>
#include "filters/Ifilter.h"

class FilterQueue {
	public:
		FilterQueue();
		~FilterQueue();

		void insertFilter(IFilter* filter);

		void filter(int bufsize, float* buffer);

	private:
		std::vector<IFilter*> _filters;
};

#endif

