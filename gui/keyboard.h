#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "Icontrol.h"
#include "../synths/Isynth.h"

class Keyboard: public IControl {
	public:
		Keyboard(SafePtr<IControl> parent, int x, int y, int h);

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);

		bool keyPress(SDLKey sym);
		bool keyRelease(SDLKey sym);

		void setSynth(SafePtr<ISynth> synth) { _synth = synth; }
		void setShift(int shift) { _shift = shift; }
		int getShift() { return _shift; }
	private:
		int keySymToNote(SDLKey sym);

		SDL_Surface* _kww;
		SDL_Surface* _kwn;
		SDL_Surface* _kbn;

		SafePtr<ISynth> _synth;

		int _shift;
		int _lastNote;
};

#endif

