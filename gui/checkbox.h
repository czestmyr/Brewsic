#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "Icontrol.h"
#include "common/propertyobserver.h"
#include "common/property.h"

class Checkbox: public IControl, PropertyObserver<bool> {
	public:
		Checkbox(SafePtr<IControl> parent, int x, int y, int w, int h, Property<bool>* prop = NULL);
		~Checkbox();

                const char* controlClassName() { return "Checkbox"; }

		void draw(SDL_Surface* surf, int x, int y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);

	private:
		void propertyChanged();

		void setValueInternal(bool value, bool byAction = false);

		bool _pressed;
		bool _value;

};

#endif

