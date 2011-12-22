#ifndef _TRIOSC_OBSERVER_H_
#define _TRIOSC_OBSERVER_H_

#include "common/Iobserver.h"

class TripleOscillator;

class TriOscObserver: public IObserver {
	public:
		TriOscObserver(TripleOscillator* parent, int action);
		void signal();

	private:
		TripleOscillator* _parent;
		int _action;
};

#endif

