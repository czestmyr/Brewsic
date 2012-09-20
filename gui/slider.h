#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "Icontrol.h"
#include "common/propertyobserver.h"
#include "common/property.h"

class Slider: public IControl, PropertyObserver<float> {
	public:
		Slider(SafePtr<IControl> parent, int x, int y, int h, float min, float max, Property<float>* prop = NULL, int bh = 10);
		~Slider();

                const char* controlClassName() { return "Slider"; }

		void redim(int x, int y, int w, int h);
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

		int _button_y;

		int _slider_button_height;
};

#endif

