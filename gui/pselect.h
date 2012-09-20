#ifndef _PICTURE_SELECTOR_H_
#define _PICTURE_SELECTOR_H_

#include "Icontrol.h"
#include "common/property.h"
#include "common/propertyobserver.h"
#include <vector>

class PictureSelector: public IControl, PropertyObserver<int> {
	public:
		PictureSelector(SafePtr<IControl> parent, int x, int y, int w, int h, Property<int>* prop);
		~PictureSelector();

                const char* controlClassName() { return "Pselect"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);

		void addPicture(const char* filename);

	protected:
		SDL_Rect _size;

		std::vector<SDL_Surface*> _pics;
};

#endif

