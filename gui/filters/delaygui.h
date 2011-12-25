#ifndef _DELAY_GUI_H
#define _DELAY_GUI_H

#include "gui/window.h"
#include "filters/delay.h"
#include "gui/slider.h"

class DelayGui: public Window {
	public:
		DelayGui(SafePtr<IControl> parent, DelayFilter* delay);
		
	private:
		DelayFilter* _delay;
};

#endif

