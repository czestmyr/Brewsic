#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <list>
#include "signals.h"

template <class T>
class Property {
	public:
		Property() {}
		Property(const T& init): data(init) {}
		Property(const T& init, Action onChange, Action onDelete = Action()) {
			data = init;
			addActions(onChange, onDelete);
		}

		~Property() {
                        _on_delete();
		}

		operator const T&() const { return data; }
		const T* operator->() const { return &data; }
		const T& operator=(const T& newVal) {
			data = newVal;
		        _on_change();
			return data;
		}

                void addActions(Action onChange, Action onDelete = Action()) {
                        _on_change.addAction(onChange);
                        if (onDelete.isValid())
        			_on_delete.addAction(onDelete);
		}

		void removeActions(Action onChange, Action onDelete = Action()) {
                        _on_change.removeAction(onChange);
                        if (onDelete.isValid())
                              _on_delete.removeAction(onDelete);
		}
	private:
		T data;

                Signal _on_change;
                Signal _on_delete;

                // Copying properties is forbidden due to unclear semantics in case of actions.
                // If you want to copy a property, do it manually instead.
                Property(const Property<T>& other) {}
};

#endif

