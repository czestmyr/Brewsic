#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "Icontrol.h"
#include "common/property.h"
#include "common/propertyobserver.h"

class Wheel: public IControl, PropertyObserver<float> {
	public:
		Wheel(SafePtr<IControl> parent, int x, int y, int w, int h, float min, float max, Property<float>* prop = NULL);
		~Wheel();

                const char* controlClassName() { return "Wheel"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		bool mouseMove(int x, int y, int dx, int dy);

		float getValue();
		void setValue(float val);

	private:
		void propertyChanged();

		void setValueInternal(float val, bool byAction = false);

		float _min;
		float _max;
		float _value;
		float _inc;

		int _pressed_y;
		float _pressed_value;
		bool _pressed;
};

#endif

