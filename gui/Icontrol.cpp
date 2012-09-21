#include "gui/Icontrol.h"

//Debug
#include <iostream>

void IControl::dumpRegisteredControls() {
        std::cout << "List of registered controls:" << std::endl;

        std::map<long, IControl*>::iterator it = registeredControls.begin();
        while (it != registeredControls.end()) {
          std::cout << "  " << it->second->controlClassName() << " " << it->second->controlId <<
            " (" << it->second->_this_ref_ptr->getCount() << " refs)" << std::endl;

	  std::list< SafePtr<IControl> >::iterator children_it = it->second->_children.begin();
          while (children_it != it->second->_children.end()) {
            std::cout << "    " << (*children_it)->controlId << std::endl;
            ++children_it;
          }

          ++it;
        }
}

std::map<long, IControl*> IControl::registeredControls;

long IControl::ctlCounter = 0;

IControl::IControl(SafePtr<IControl> parent): _x(0), _y(0), _w(0), _h(0),
	_delete_me(false), _margins(5), _pack_horizontally(false), _auto_packing(false),
	_packable(true), _focusable(false), _prefered_h(0), _prefered_w(0), _pack_weight(1),
        _dirty(true) {
	_this_ref_ptr = new RefPtr<IControl>(this);
	if (parent) {
          _parent = parent.get();
          _parent->adopt(safePtr());
        } else {
          _parent = NULL;
        }
        registerControl(this);
}

int IControl::xToParent(int x) {
	if (_parent) {
		return _parent->_x + _parent->xToParent(x);
	} else return x;
}

int IControl::yToParent(int y) {
	if (_parent) {
		return _parent->_y + _parent->yToParent(y);
	} else return y;
}

int IControl::xFromParent(int x) {
	if (_parent) {
		return _parent->xFromParent(x) - _parent->_x;
	} else return x;
}

int IControl::yFromParent(int y) {
	if (_parent) {
		return _parent->yFromParent(y) - _parent->_y;
	} else return y;
}

void IControl::recursiveDraw(SDL_Surface* surf, int orig_x, int orig_y) {
	draw(surf, orig_x, orig_y);

	_rit = _children.rbegin();
	while (_rit != _children.rend()) {
		(*_rit)->recursiveDraw(surf, orig_x + _x, orig_y + _y);
		++_rit;
	}

//        TODO: implement on-demand redraw
//        _dirty = false;
}

SafePtr<IControl> IControl::recursiveLeftPress(int x, int y) {
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

bool IControl::recursiveRightPress(int x, int y) {
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

bool IControl::recursiveLeftRelease(int x, int y) {
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

bool IControl::recursiveRightRelease(int x, int y) {
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

bool IControl::recursiveMouseMove(int x, int y, int dx, int dy) {
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

bool IControl::recursiveKeyPress(SDLKey sym) {
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

bool IControl::recursiveKeyRelease(SDLKey sym) {
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

void IControl::redim(int x, int y, int w, int h) {
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

void IControl::rawRedim(int x, int y, int w, int h) {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

void IControl::setPreferedSize(int w, int h, int weight) {
	_prefered_w = w;
	_prefered_h = h;
	_pack_weight = weight;
}

void IControl::packHorizontally(int margins) {
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
	int y, w, h;
	int maxh = getYMax() - getYMin();
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
			if ((*_it)->_prefered_h) {
				h = (*_it)->_prefered_h < maxh ? (*_it)->_prefered_h : maxh;
				if ((*_it)->_prefered_h < maxh) {
					h = (*_it)->_prefered_h;
					y = getYMin() + (getYMax() - getYMin() - h)/2;
				} else {
					h = maxh;
					y = getYMin();
				}
			} else {
				h = maxh;
				y = getYMin();
			}
			(*_it)->redim(x, y, w, h);
			x += w + margins;
		}
		++_it;
	}
}

void IControl::packVertically(int margins) {
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
	int x, w, h;
	int maxw = getXMax() - getXMin();
	while (_it != _children.end()) {
		if ((*_it)->_packable) {
			if ((*_it)->_prefered_h && obey_preference) {
				h = (*_it)->_prefered_h;
			} else {
				h = total_space * (*_it)->_pack_weight / total_weight;
				total_space -= h;
				total_weight -= (*_it)->_pack_weight;
			}
			if ((*_it)->_prefered_w) {
				w = (*_it)->_prefered_w < maxw ? (*_it)->_prefered_w : maxw;
				if ((*_it)->_prefered_w < maxw) {
					w = (*_it)->_prefered_w;
					x = getXMin() + (getXMax() - getXMin() - w)/2;
				} else {
					w = maxw;
					x = getXMin();
				}
			} else {
				w = maxw;
				x = getXMin();
			}
			(*_it)->redim(x, y, w, h);
			y += h + margins;
		}
		++_it;
	}
}

void IControl::markDirty() {
        _dirty = true;
        if (_parent)
                _parent->_dirty = true;
}

