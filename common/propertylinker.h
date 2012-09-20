#ifndef _PROPERTY_LINKER_H_
#define _PROPERTY_LINKER_H_

#include "property.h"
#include "propertyobserver.h"
#include <list>

/// This class links the values of several properties together.
/// If one oproperty changes, the change is propagated to the others.
template <class T> class PropertyLinker {
  private:
    class LinkedPropertyObserver;

  public:
    PropertyLinker(): _linking(false) {}

    void addProperty(Property<T>* newProp) {
      _property_observers.push_back(LinkedPropertyObserver(this, newProp));

      if (_property_observers.size() > 1) {
        _property_observers.back().setLinkedProperty(
          _property_observers.front().getLinkedProperty()
        );
      }
    }

  private:
    void changeValues(LinkedPropertyObserver* originator) {
      T value = originator->getLinkedProperty();
      typename std::list<PropertyLinker<T>::LinkedPropertyObserver>::iterator it = _property_observers.begin();

      _linking = true;
      while (it != _property_observers.end()) {
        if (&(*it) != originator) {
          (*it).setLinkedProperty(value);
        }
        ++it;
      }
      _linking = false;
    }

    // Prevent reentry
    bool _linking;
    friend class LinkedPropertyObserver;

    class LinkedPropertyObserver: public PropertyObserver<T> {
      public:
        LinkedPropertyObserver(PropertyLinker<T>* parent, Property<T>* prop): PropertyObserver<T>(prop), _parent(parent) {}

        void setLinkedProperty(const T& value) {
          setProp(value);
        }

        T getLinkedProperty() {
          return this->getProp();
        }

        void propertyChanged() {
          if (!_parent->_linking)
            _parent->changeValues(this);
        }
      private:
        PropertyLinker<T>* _parent;
    };

    std::list<LinkedPropertyObserver> _property_observers;
};

#endif

