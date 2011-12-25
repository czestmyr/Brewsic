#ifndef _INTERFACE_CONTROL_H_
#define _INTERFACE_CONTROL_H_

#include <SDL.h>
#include <list>
#include "draw/SDL_draw.h"

class IControl {
	public:
		IControl(IControl* parent): _parent(parent), _x(0), _y(0), _w(0), _h(0), _delete_me(false){
			if (_parent) _parent->adopt(this);
		}

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
		virtual bool mouseMove(int x, int y, int dx, int dy) { return false; }
		virtual bool keyPress(SDLKey sym) { return false; }
		virtual bool keyRelease(SDLKey sym) { return false; }

		virtual bool inside(int x, int y) { 
			if (x >= _x && x <= _x + _w && y >= _y && y <= _y + _h) return true;
			return false;
		}

		virtual int getXMin() { return 0; }
		virtual int getYMin() { return 0; }
		virtual int getXMax() { return _w; }
		virtual int getYMax() { return _h; }

		int xToParent(int x) {
			if (_parent) {
				return _parent->_x + _parent->xToParent(x);
			} else return x;
		}

		int yToParent(int y) {
			if (_parent) {
				return _parent->_y + _parent->yToParent(y);
			} else return y;
		}

		int xFromParent(int x) {
			if (_parent) {
				return _parent->xFromParent(x) - _parent->_x;
			} else return x;
		}

		int yFromParent(int y) {
			if (_parent) {
				return _parent->yFromParent(y) - _parent->_y;
			} else return y;
		}

		void recursiveDraw(SDL_Surface* surf, int orig_x, int orig_y) {
			draw(surf, orig_x, orig_y);

			_rit = _children.rbegin();
			while (_rit != _children.rend()) {
				(*_rit)->recursiveDraw(surf, orig_x + _x, orig_y + _y);
				++_rit;
			}
		}

		IControl* recursiveLeftPress(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				IControl* ptr;
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
				ptr = (*_it)->recursiveLeftPress(x-_x, y-_y);
				if (ptr) {
					return ptr;
				}
				++_it;
			}

			if (leftPress(x, y)) {
				return this;
			} else {
				return NULL;
			}
		}

		bool recursiveRightPress(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
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
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
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
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
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

		bool recursiveMouseMove(int x, int y, int dx, int dy) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
				if ((*_it)->recursiveMouseMove(x-_x, y-_y, dx, dy)) {
					return true;
				}
				++_it;
			}

			if (mouseMove(x, y, dx, dy)) {
				return true;
			} else {
				return false;
			}
		}

		bool recursiveKeyPress(SDLKey sym) {
			if (keyPress(sym)) {
				return true;
			}

			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveKeyPress(sym)) {
					return true;
				}
				++_it;
			}

			return false;
		}

		bool recursiveKeyRelease(SDLKey sym) {
			if (keyRelease(sym)) {
				return true;
			}

			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->recursiveKeyRelease(sym)) {
					return true;
				}
				++_it;
			}

			return false;
		}

		void redim(int x, int y, int w, int h) {
			_x = x; _y = y; _w = w; _h = h;

			if (_parent) {
				if (_w > _parent->getXMax() - _parent->getXMin())
					_w = _parent->getXMax() - _parent->getXMin();
				if (_h > _parent->getYMax() - _parent->getYMin())
					_h = _parent->getYMax() - _parent->getYMin();
				if (_x < _parent->getXMin()) _x = _parent->getXMin();
				if (_y < _parent->getYMin()) _y = _parent->getYMin();
				if (_x + _w > _parent->getXMax()) _x = _parent->getXMax() - _w;
				if (_y + _h > _parent->getYMax()) _y = _parent->getYMax() - _h;		
			}
		}

		void adopt(IControl* child) { _children.push_back(child); }
		void leave(IControl* child) { _children.remove(child); }

		int getW() { return _w; }
		int getH() { return _h; }
		int getX() { return _x; }
		int getY() { return _y; }

		void toFront() { if (_parent) _parent->childToFront(this); }
		void childToFront(IControl* child) {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it) == child) {
					_children.remove(child);
					_children.push_front(child);
					return;
				}
				++_it;
			}
		}

		void deleteMe() { _delete_me = true; }
		void recursiveCleanup() {
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->_delete_me) {
					delete (*_it);
					_it = _children.erase(_it);
				} else {
					(*_it)->recursiveCleanup();
					++_it;
				}
			}
		}
	protected:
		int _x;
		int _y;
		int _w;
		int _h;

		bool _delete_me;

		std::list<IControl*> _children;
		std::list<IControl*>::iterator _it;
		std::list<IControl*>::reverse_iterator _rit;
		IControl* _parent;
};

#endif

