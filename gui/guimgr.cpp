#include "guimgr.h"

void GuiMgr::leftPress(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if (!(*it)->inside(x,y)) { ++it; continue; }
		IControl* ptr;
		ptr = (*it)->recursiveLeftPress(x,y);
		if (ptr) {
			_drag = ptr;
			_drag->toFront();
			break;
		}
		++it;
	}
}

void GuiMgr::rightPress(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if (!(*it)->inside(x,y)) { ++it; continue; }
		if ((*it)->recursiveRightPress(x,y)) break;
		++it;
	}
}

void GuiMgr::leftRelease(int x, int y) {
	if (_drag) {
		int x_loc = _drag->xFromParent(x);
		int y_loc = _drag->yFromParent(y);
		if (_drag->leftRelease(x_loc, y_loc) &&
		    _drag->inside(x_loc, y_loc)) {
			_drag = NULL;
			return;
		}
		_drag = NULL;
	}

	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if (!(*it)->inside(x,y)) { ++it; continue; }
		if ((*it)->recursiveLeftRelease(x,y)) break;
		++it;
	}
}

void GuiMgr::rightRelease(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if (!(*it)->inside(x,y)) { ++it; continue; }
		if ((*it)->recursiveRightRelease(x,y)) break;
		++it;
	}
}

void GuiMgr::mouseMove(int x, int y, int dx, int dy) {
	if (_drag) {
		int x_loc = _drag->xFromParent(x);
		int y_loc = _drag->yFromParent(y);
		if (_drag->mouseMove(x_loc, y_loc, dx, dy) &&
		    _drag->inside(x_loc, y_loc)) {
			return;
		}
	}

	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if (!(*it)->inside(x,y)) { ++it; continue; }
		if ((*it)->recursiveMouseMove(x,y,dx,dy)) break;
		++it;
	}
}

void GuiMgr::keyPress(SDLKey sym) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveKeyPress(sym))
			return;
		++it;
	}
}

void GuiMgr::keyRelease(SDLKey sym) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveKeyRelease(sym))
			return;
		++it;
	}
}

void GuiMgr::draw(SDL_Surface* surf) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		(*it)->recursiveDraw(surf, 0, 0);
		++it;
	}
}

void GuiMgr::cleanup() {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		(*it)->recursiveCleanup();
		++it;
	}
}

