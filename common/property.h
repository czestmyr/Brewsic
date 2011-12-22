#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <list>
#include "Iobserver.h"

template <class T>
class Property {
	public:
		Property(const T& init): T(init) {}
		Property(const T& init, IObserver* obs) {
			data = init;
			addObserver(obs);
		}

		operator const T&() const { return data; }
		const T& operator=(const T& newVal) {
			data = newVal;
		
			std::list<IObserver*>::iterator it = observers.begin();
			while (it != observers.end()) {
				(*it)->signal();
				++it;
			}
		
			return data;
		}

		void addObserver(IObserver* observer) {
			observers.push_back(observer);
		}
	private:
		T data;

		std::list<IObserver*> observers;
};

#endif

