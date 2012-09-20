#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include <cstddef>
#include <list>

// An abstraction of an object and it's method.
// Using this abstraction, the method on an object can be called without
// knowing anything about the object or the method.
// The method cannot be called directly. Instead, a proxy method is used.
// This proxy method can be created using the ACTION macro.
class Action {
	public:
		Action(): _p(NULL), _o(NULL) {};
		Action(void* object, void (*staticProxy)(void*)):
			_o(object), _p(staticProxy) {}
                bool isValid() const {
                        return (_p && _o);
                }
		void operator() () const{
			if (isValid())
				_p(_o);
		}
                bool operator==(const Action& other) {
                        return _p == other._p && _o == other._o;
                }
	private:
		void* _o;
		void (*_p)(void*);
};

class Signal {
  public:
    void operator() () const {
      std::list<Action>::const_iterator it = _actions.begin();
      while (it != _actions.end()) {
        (*it)();
        ++it;
      }
    }

    void addAction(Action action) { _actions.push_back(action); }
    void removeAction(Action action) {
      std::list<Action>::iterator it = _actions.begin();
      while (it != _actions.end()) {
        if (*it == action)
          it = _actions.erase(it);
        else
          ++it;
      }
    }
  private:
    std::list<Action> _actions;
};

#define ACTION(className, method) \
		static void method##StaticProxyMethod (void* obj){\
                        ((className *) obj)->method();\
		}\
		Action method##Action() {\
			return Action(this, className::method##StaticProxyMethod);\
		}\

#endif

