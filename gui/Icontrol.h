#ifndef _INTERFACE_CONTROL_H_
#define _INTERFACE_CONTROL_H_

#include <SDL.h>
#include <list>
#include "draw/SDL_draw.h"
#include "common/pointers.h"

#define NULL_CTL (IControl*) NULL
#define safe_new(obj) (new obj)->dropAndSafePtr()

class IControl {
	public:
		static long ctlCounter;
		static void incCounter() { ctlCounter++; }
		static void decCounter() { ctlCounter--; }

		IControl(SafePtr<IControl> parent): _parent(parent), _x(0), _y(0), _w(0), _h(0),
			_delete_me(false), _margins(5), _pack_horizontally(false), _auto_packing(false),
       			_packable(true), _focusable(false), _prefered_h(0), _prefered_w(0), _pack_weight(1) {
			_this_ref_ptr = new RefPtr<IControl>(this);
			if (_parent) _parent->adopt(safePtr());
			incCounter();
		}

		virtual ~IControl() { decCounter(); }

		SafePtr<IControl> safePtr() { return SafePtr<IControl>(_this_ref_ptr); }

		//Used after construction to decrease reference count (we don't refcount ourselves)
		SafePtr<IControl> dropAndSafePtr() { SafePtr<IControl> ret(_this_ref_ptr); _this_ref_ptr->drop(); return ret; }

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

		SafePtr<IControl> recursiveLeftPress(int x, int y) {
			_it = _children.begin();
			while (_it != _children.end()) {
				SafePtr<IControl> ptr;
				if (!(*_it)->inside(x-_x,y-_y)) { ++_it; continue; }
				ptr = (*_it)->recursiveLeftPress(x-_x, y-_y);
				if (ptr) {
					return ptr;
				}
				++_it;
			}

			if (leftPress(x, y)) {
				return _this_ref_ptr;
			} else {
				return NULL_CTL;
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
			bool resize = false;
			if (_w != w || _h != h) resize = true;
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

			if (resize && _auto_packing && _children.size() != 0) {
				if (_pack_horizontally)
					packHorizontally(_margins);
				else
					packVertically(_margins);
			}
		}

		void setPreferedSize(int w, int h, int weight) {
			_prefered_w = w;
			_prefered_h = h;
			_pack_weight = weight;
		}

		void packHorizontally(int margins) {
			_auto_packing = true;
			_pack_horizontally = true;
			_margins = margins;

			int margin_space = 0;
			int total_space = 0;
			int total_weight = 0;
			int weight_of_fixed = 0;

			bool obey_preference = true;
			bool some_nonfixed = false;
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->_packable) {
					if ((*_it)->_prefered_w) {
						total_space -= (*_it)->_prefered_w;
						weight_of_fixed += (*_it)->_pack_weight;
					} else {
						some_nonfixed = true;
						total_weight += (*_it)->_pack_weight;
					}
					margin_space += margins;
				}
				_it++;
			}

			total_space += getXMax() - getXMin();
			total_space -= margin_space - margins;  // There is one less margins than packable controls
			if (total_space < 0) {  // This means that fixed-size controls cannot fit -> all controls will be non-fixed
				obey_preference = false;
				some_nonfixed = true;
				total_space = getXMax() - getXMin() - margin_space;
				total_weight += weight_of_fixed;
			}

			_it = _children.begin();
			int x = getXMin();
			if (!some_nonfixed) x += total_space / 2;  // If all controls are fixed, center them
			int y = getYMin();
			int h = getYMax() - getYMin();
			while (_it != _children.end()) {
				if ((*_it)->_packable) {
					int w;
					if ((*_it)->_prefered_w && obey_preference) {
						w = (*_it)->_prefered_w;
					} else {
						w = total_space * (*_it)->_pack_weight / total_weight;
						total_space -= w;
						total_weight -= (*_it)->_pack_weight;
					}
					(*_it)->redim(x, y, w, h);
					x += w + margins;
				}
				++_it;
			}
		}

		void packVertically(int margins) {
			_auto_packing = true;
			_pack_horizontally = false;
			_margins = margins;

			int margin_space = 0;
			int total_space = 0;
			int total_weight = 0;
			int weight_of_fixed = 0;

			bool obey_preference = true;
			bool some_nonfixed = false;
			_it = _children.begin();
			while (_it != _children.end()) {
				if ((*_it)->_packable) {
					if ((*_it)->_prefered_h) {
						total_space -= (*_it)->_prefered_h;
						weight_of_fixed += (*_it)->_pack_weight;
					} else {
						some_nonfixed = true;
						total_weight += (*_it)->_pack_weight;
					}
					margin_space += margins;
				}
				_it++;
			}

			total_space += getYMax() - getYMin();
			total_space -= margin_space - margins;  // There is one less margins than packable controls
			if (total_space < 0) {  // This means that fixed-size controls cannot fit -> all controls will be non-fixed
				obey_preference = false;
				some_nonfixed = true;
				total_space = getYMax() - getYMin() - margin_space;
				total_weight += weight_of_fixed;
			}

			_it = _children.begin();
			int y = getYMin();
			if (!some_nonfixed) y += total_space / 2;  // If all controls are fixed, center them
			int x = getXMin();
			int w = getXMax() - getXMin();
			while (_it != _children.end()) {
				if ((*_it)->_packable) {
					int h;
					if ((*_it)->_prefered_h && obey_preference) {
						h = (*_it)->_prefered_h;
					} else {
						h = total_space * (*_it)->_pack_weight / total_weight;
						total_space -= h;
						total_weight -= (*_it)->_pack_weight;
					}
					(*_it)->redim(x, y, w, h);
					y += h + margins;
				}
				++_it;
			}
		}

		void setPackable(bool p) { _packable = p; }

		void adopt(SafePtr<IControl> child) { _children.push_back(child); }
		void leave(SafePtr<IControl> child) { _children.remove(child); }

		int getW() { return _w; }
		int getH() { return _h; }
		int getX() { return _x; }
		int getY() { return _y; }

		void toFront() { if (_focusable && _parent) _parent->childToFront(_this_ref_ptr); }
		void childToFront(SafePtr<IControl> child) {
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

		int _prefered_w;
		int _prefered_h;
		int _pack_weight;

		bool _auto_packing;
		bool _pack_horizontally;
		int _margins;
		bool _packable;

		bool _focusable;

		bool _delete_me;

		std::list< SafePtr<IControl> > _children;
		std::list< SafePtr<IControl> >::iterator _it;
		std::list< SafePtr<IControl> >::reverse_iterator _rit;
		SafePtr<IControl> _parent;
		RefPtr<IControl>* _this_ref_ptr;
};

#endif

