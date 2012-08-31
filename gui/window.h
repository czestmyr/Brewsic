#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include "Icontrol.h"
#include "common/property.h"
#include "common/Iobserver.h"
#include "common/signals.h"
#include "gui/button.h"

#define MARGIN 3

class Window: public IControl {
	public:
		Window(SafePtr<IControl>, int x, int y, int w, int h, const char* title = "Unnamed window");
		~Window() {};

                const char* controlClassName() { return "Window"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		bool mouseMove(int x, int y, int dx, int dy);
		bool keyPress(SDLKey sym);
		bool keyRelease(SDLKey sym);

		int getYMin() { return _title_h + MARGIN; }
		int getYMax() { return _h - _status_h - MARGIN; }
		int getXMin() { return 1 + MARGIN; }
		int getXMax() { return _w-1 - MARGIN; }

		SIGNAL_DESTINATION(_close, Window, close);
		void close() { deleteMe(); }
	protected:
		std::string _name;
		std::string _status;

		int _status_h;
		int _title_h;

		bool _dragging;

		SafePtr<Button> _close_btn;
};

#endif

