#ifndef _GUIMGR_H_
#define _GUIMGR_H_

#include <vector>
#include "Icontrol.h"

class GuiMgr {
	public:
		GuiMgr(): _drag((IControl*)NULL) {}
		~GuiMgr() {}

		void leftPress(int x, int y);
		void rightPress(int x, int y);
		void leftRelease(int x, int y);
		void rightRelease(int x, int y);
		void mouseMove(int x, int y, int dx, int dy);
		void keyPress(SDLKey sym);
		void keyRelease(SDLKey sym);

		void draw(SDL_Surface* surf);
		void cleanup();

		void adoptControl(SafePtr<IControl> control) {
			_controls.push_back(control);
		}
	private:
		std::vector<SafePtr<IControl> > _controls;

		SafePtr<IControl> _drag;
};

#endif

