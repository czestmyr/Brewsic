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

		IControl(SafePtr<IControl> parent);
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

		int xToParent(int x);
		int yToParent(int y);
		int xFromParent(int x);
		int yFromParent(int y);

		void recursiveDraw(SDL_Surface* surf, int orig_x, int orig_y);
		SafePtr<IControl> recursiveLeftPress(int x, int y);
		bool recursiveRightPress(int x, int y);
		bool recursiveLeftRelease(int x, int y);
		bool recursiveRightRelease(int x, int y);
		bool recursiveMouseMove(int x, int y, int dx, int dy);
		bool recursiveKeyPress(SDLKey sym);
		bool recursiveKeyRelease(SDLKey sym);

		virtual void redim(int x, int y, int w, int h);
		void rawRedim(int x, int y, int w, int h);
		void setPreferedSize(int w, int h, int weight);
		void packHorizontally(int margins);
		void packVertically(int margins);
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
		IControl* _parent;
		RefPtr<IControl>* _this_ref_ptr;
};

#endif

