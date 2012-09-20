#ifndef _PROPERTY_TEST_H_
#define _PROPERTY_TEST_H_

#include "property.h"
#include "propertyobserver.h"

#include <iostream>

// This files shows how properties and their observers can be used

using namespace std;

class PropertyTest {
	public:
                // A class that observes an integer property and reacts to a change
		class NumberObserver: public PropertyObserver<int> {
			public:
				NumberObserver(Property<int>* prop = NULL): PropertyObserver<int>(prop) {}
				void propertyChanged() {
			                cout << "The current value of number is " << getProp() << endl;
                                }
                                void setValue(int val) { setProp(val); }
		};
	public:
                static void runTest() {
		        Property<int>* _number;
		        NumberObserver _num_obs;

                        // The number observer now does not point to a property, but setting it is safe:
                        _num_obs.setValue(10);
                        // We create the property and supply it to the number observer
                        _number = new Property<int>(0);
                        _num_obs = NumberObserver(_number);
                        _num_obs.setValue(42);
                        // Deleting the property now is also safe
                        delete _number;
                        _num_obs.setValue(13);
                }
};

#endif

