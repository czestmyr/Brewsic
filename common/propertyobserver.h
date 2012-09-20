#ifndef _PROPERTY_OBSERVER_H_
#define _PROPERTY_OBSERVER_H_

#include "signals.h"
#include "property.h"

template <class T> class PropertyObserver {
  public:
    PropertyObserver(Property<T>* prop = NULL): _prop(prop) {
      if (_prop)
        _prop->addActions(propertyChangedAction(), propertyDeletedMetaAction());
    }

    PropertyObserver(const PropertyObserver<T>& other): _prop(other._prop) {
      if (_prop)
        _prop->addActions(propertyChangedAction(), propertyDeletedMetaAction());
    }

    ~PropertyObserver() {
      if (_prop)
        _prop->removeActions(propertyChangedAction(), propertyDeletedMetaAction());
    }

    PropertyObserver<T>& operator= (const PropertyObserver<T>& other) {
      if (_prop)
        _prop->removeActions(propertyChangedAction(), propertyDeletedMetaAction());
      _prop = other._prop;
      if (_prop)
        _prop->addActions(propertyChangedAction(), propertyDeletedMetaAction());
    }

  protected:
    ACTION(PropertyObserver<T>, propertyChanged);
    ACTION(PropertyObserver<T>, propertyDeletedMeta);
    virtual void propertyChanged() = 0;
    virtual void propertyDeleted() {};
    void propertyDeletedMeta() {
      _prop = NULL;
      this->propertyDeleted();
    }

    T getProp() {
      if (_prop)
        return *_prop;
      else
        return T();
    }

    void setProp(const T& value) {
      if (_prop)
        *_prop = value;
    }

    bool propValid() {
      return (_prop);
    }

  private:
    Property<T>* _prop;
};

#endif

