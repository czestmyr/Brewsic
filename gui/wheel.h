#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "Icontrol.h"

class Wheel: public IControl {
	public:
		Wheel(IControl* parent, int x, int y, int w, int h, int min, int max, void* data = NULL);

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		bool mouseMove(int x, int y, int dx, int dy);

		void setCallback(void (*callback)(void* data));
		int getValue();
		void setValue(int val);
	private:
		void (*_callback)(void* data);
		void* _data;

		int _r;

		int _min;
		int _max;
		int _value;
		float _inc;

		int _pressed_y;
		int _pressed_value;
		bool _pressed;
};

#endif

