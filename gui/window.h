#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include "Icontrol.h"
#include "common/property.h"
#include "common/Iobserver.h"
#include "common/signals.h"

class Button;

class Window: public IControl {
	public:
		Window(SafePtr<IControl>, int x, int y, int w, int h, const char* title = "Unnamed window");
		~Window() {};
		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		bool mouseMove(int x, int y, int dx, int dy);
		bool keyPress(SDLKey sym);
		bool keyRelease(SDLKey sym);

		int getYMin() { return _title_h; }
		int getYMax() { return _h - _status_h; }
		int getXMin() { return 1; }
		int getXMax() { return _w-1; }

		SIGNAL_DESTINATION(_close, Window, close);
		void close() { deleteMe(); }
	protected:
		std::string _name;
		std::string _status;

		int _status_h;
		int _title_h;

		bool _dragging;

		Button* _close_btn;
};

#endif

