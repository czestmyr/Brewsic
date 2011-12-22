#ifndef _PROPERTY_TEST_H_
#define _PROPERTY_TEST_H_

#include "common/property.h"
#include "common/Iobserver.h"

#include <iostream>

using namespace std;

class PropertyTest {
	public:
		class NumberObserver: public IObserver {
			public:
				NumberObserver(PropertyTest* parent): _parent(parent) {}
				void signal() { _parent->numberChanged(); }
			private:
				PropertyTest* _parent;
		};
	public:
		PropertyTest(): _num_obs(this), _number(0, &_num_obs) {}

		void setNumber(int number) { _number = number; }

	private:
		NumberObserver _num_obs;
		Property<int> _number;

		void numberChanged() {
			cout << "The current value of number is " << _number << endl;
		}
};

#endif

