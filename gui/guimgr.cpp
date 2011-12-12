#include "guimgr.h"

void GuiMgr::leftPress(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveLeftPress(x,y)) break;
		++it;
	}
}

void GuiMgr::rightPress(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveRightPress(x,y)) break;
		++it;
	}
}

void GuiMgr::leftRelease(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveLeftRelease(x,y)) break;
		++it;
	}
}

void GuiMgr::rightRelease(int x, int y) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveRightRelease(x,y)) break;
		++it;
	}
}

void GuiMgr::mouseMove(int dx, int dy) {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->recursiveMouseMove(x,y)) break;
		++it;
	}
}

void GuiMgr::draw() {
	std::vector<IControl*>::iterator it = _controls.begin();
	while (it != _controls.end()) {
		(*it)->recursiveDraw(0,0);
		++it;
	}
}

