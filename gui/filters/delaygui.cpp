#include "delaygui.h"

DelayGui::DelayGui(IControl* parent, DelayFilter* delay):
	_delay(delay),
	Window(parent, 0, 0, 50, 150, "Delay") {
	_atten_slider = new Slider(this, 15, 25, 100, 0.0, 1.0, &_delay->_atten);
}

