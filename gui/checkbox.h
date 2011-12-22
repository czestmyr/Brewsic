#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "Icontrol.h"
#include "common/Iobserver.h"
#include "common/property.h"

class Checkbox: public IControl, IObserver {
	public:
		Checkbox(IControl* parent, int x, int y, int w, int h, Property<bool>* prop = NULL);

		void draw(SDL_Surface* surf, int x, int y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);

		void signal();
		void disconnect();
	private:
		void setValueInternal(bool value);

		bool _pressed;
		bool _value;
		Property<bool>* _prop;

};

#endif

