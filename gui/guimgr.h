#ifndef _GUIMGR_H_
#define _GUIMGR_H_

#include <vector>
#include "Icontrol.h"

class GuiMgr {
	public:
		GuiMgr(): _drag(0); {}
		~GuiMgr() {
			std::vector<IControl*>::iterator it = _controls.begin();
			while (it != _controls.end()) {
				delete (*it);
				++it;
			}
		}

		void leftPress(int x, int y);
		void rightPress(int x, int y);
		void leftRelease(int x, int y);
		void rightRelease(int x, int y);
		void mouseMove(int dx, int dy);

		void draw();

		void adoptControl(IControl* control) {
			_controls.push_back(control);
		}
	private:
		std::vector<IControl*> _controls;

		IControl* _drag;
};

#endif

