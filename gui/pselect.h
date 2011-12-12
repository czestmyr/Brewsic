#ifndef _PICTURE_SELECTOR_H_
#define _PICTURE_SELECTOR_H_

#include "Icontrol.h"
#include <vector>

class PictureSelector: public IControl {
	public:
		PictureSelector(IControl* parent, int x, int y, int w, int h, void* data = NULL);
		~PictureSelector();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);

		void addPicture(const char* filename);

		void setCallback(void (*callback)(void* data));
		int getSelection() { return _selection; }
	protected:
		SDL_Rect _size;

		void (*_callback)(void* data);
		void* _data;

		std::vector<SDL_Surface*> _pics;
		int _selection;
};

#endif

