#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "Icontrol.h"
#include "common/Iobserver.h"
#include "common/property.h"

class Wheel: public IControl, IObserver {
	public:
		Wheel(IControl* parent, int x, int y, int w, int h, float min, float max, Property<float>* prop = NULL);

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		bool mouseMove(int x, int y, int dx, int dy);

		float getValue();
		void setValue(float val);

		void signal();
		void disconnect();
	private:
		void setValueInternal(float val, bool bySignal = false);

		int _r;

		float _min;
		float _max;
		float _value;
		Property<float>* _prop;
		float _inc;

		int _pressed_y;
		float _pressed_value;
		bool _pressed;
};

#endif

