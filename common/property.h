#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <list>
#include "Iobserver.h"

template <class T>
class Property {
	public:
		Property(const T& init): data(init) {}
		Property(const T& init, IObserver* obs) {
			data = init;
			addObserver(obs);
		}

		~Property() {
			std::list<IObserver*>::iterator it = observers.begin();
			while (it != observers.end()) {
				(*it)->disconnect();
				++it;
			}
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

		void removeObserver(IObserver* observer) {
			std::list<IObserver*>::iterator it = observers.begin();
			while (it != observers.end()) {
				if (*it == observer)
					it = observers.erase(it);
				else
					++it;
			}
		}
	private:
		T data;

		std::list<IObserver*> observers;
};

#endif

