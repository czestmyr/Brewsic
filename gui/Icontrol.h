#ifndef _INTERFACE_CONTROL_H_
#define _INTERFACE_CONTROL_H_

#include <SDL.h>
#include <vector>

class IControl {
	public:
		IControl(IControl* parent): _parent(parent) {}
		virtual ~IControl() {
			_it = _children.begin();
			while (_it != _children.end()) {
				delete (*_it);
				++_it;
			}
		}

		virtual void draw(SDL_Surface* surf, int orig_x, int orig_y) { return; }

		virtual bool leftPress(int x, int y) { return false; }
		virtual bool rightPress(int x, int y) { return false; }
		virtual bool leftRelease(int x, int y) { return false; }
		virtual bool rightRelease(int x, int y) { return false; }
		virtual bool mouseMove(int dx, int dy) { return false; }

		void recursiveDraw(SDL_Surface* surf, int orig_x, int orig_y) {
			draw(surf, orig_x, orig_y);

			_it = _children.begin();
			while (_it != _children.end()) {
				(*_it)->draw(surf, orig_x + _x, orig_y + _y);
				++_it;
			}
		}

		bool recursiveLeftPress(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveLeftPress(x-_x, y-_y)) {
					return true;
				}
				++_it;
			}

			if (leftPress(x, y)) {
				return true;
			} else {
				return false;
			}
		}

		bool recursiveRightPress(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveRightPress(x-_x, y-_y)) {
					return true;
				}
				++_it;
			}

			if (rightPress(x, y)) {
				return true;
			} else {
				return false;
			}
		}

		bool recursiveLeftRelease(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveLeftRelease(x-_x, y-_y)) {
					return true;
				}
				++_it;
			}

			if (leftRelease(x, y)) {
				return true;
			} else {
				return false;
			}
		}

		bool recursiveRightRelease(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveRightRelease(x-_x, y-_y)) {
					return true;
				}
				++_it;
			}

			if (rightRelease(x, y)) {
				return true;
			} else {
				return false;
			}
		}

		bool recursiveMouseMove(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveMouseMove(x-_x, y-_y)) {
					return true;
				}
				++_it;
			}

			if (mouseMove(x, y)) {
				return true;
			} else {
				return false;
			}
		}

		void redim(int x, int y, int w, int h) {
			_x = x; _y = y; _w = w; _h = h;
		}

		void adopt(IControl* child) { _children.push_back(child); }
	protected:
		int _x;
		int _y;
		int _w;
		int _h;

		std::vector<IControl*> _children;
		std::vector<IControl*>::iterator _it;
		IControl* _parent;
};

#endif

