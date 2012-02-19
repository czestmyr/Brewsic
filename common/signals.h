#ifndef _SIGNALS_H_
#define _SIGNALS_H_

class Signal {
	public:
		Signal(): _p(NULL), _o(NULL) {};
		Signal(void* object, void (*staticProxy)(void*)):
			_o(object), _p(staticProxy) {}
		void operator() (){
			if (_p && _o)
				_p(_o);
		}
	private:
		void* _o;
		void (*_p)(void*);
};

#define SIGNAL_DESTINATION(sigName, className, method) class sigName##SignalClass {\
	public:\
		static void staticProxyMethod (sigName##SignalClass* obj){\
			obj->proxyMethod();\
		}\
		sigName##SignalClass(className* parent): _p(parent) {};\
		void proxyMethod() {\
			_p->method();\
		}\
		Signal getSignal() {\
			return Signal(this, (void(*)(void*))sigName##SignalClass::staticProxyMethod);\
		}\
	private:\
		className* _p;\
}; sigName##SignalClass sigName;

#endif

