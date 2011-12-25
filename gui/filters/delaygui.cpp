#include "delaygui.h"

DelayGui::DelayGui(SafePtr<IControl> parent, DelayFilter* delay):
	_delay(delay),
	Window(parent, 0, 0, 50, 150, "Delay") {
	new Slider(this->safePtr(), 15, 25, 100, 0.0, 1.0, &_delay->_atten);
}

