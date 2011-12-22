#include "trioscobserver.h"
#include "triosc.h"

TriOscObserver::TriOscObserver(TripleOscillator* parent, int action): _parent(parent), _action(action) {
	switch (action) {
		case PROP_FIRST:
			_parent->_first.addObserver(this);
		break;
		case PROP_SECOND:
			_parent->_second.addObserver(this);
		break;
		case PROP_SHIFT:
			_parent->_shift.addObserver(this);
		break;
	}
}

void TriOscObserver::signal() {
	//TODO: So far, it suffices to change the values. In the future, something will be done here.
	switch (_action) {
		case PROP_FIRST:
		break;
		case PROP_SECOND:
		break;
		case PROP_SHIFT:
		break;
	}
}
