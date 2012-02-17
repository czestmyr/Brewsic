#include "filterqueue.h"

FilterQueue::FilterQueue() {

}

FilterQueue::~FilterQueue() {
	for (int i = 0; i < _filters.size(); ++i) {
		delete _filters[i];
	}
}

void FilterQueue::insertFilter(IFilter* filter) {
	_filters.push_back(filter);
}

void FilterQueue::filter(int bufsize, float* buffer) {

}

