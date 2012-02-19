#ifndef _INTERFACE_SYNTH_H__
#define _INTERFACE_SYNTH_H__

#include "gui/Icontrol.h"

class ISynth {
	public:
		virtual void startNote(int noteId, float frequency) = 0;
		virtual void stopNote(int noteId) = 0;

		virtual void generateOutput() = 0;
		virtual float* getBuffer() = 0;

		virtual const char* getClassName() = 0;

		virtual void createGui(SafePtr<IControl> parent) = 0;
};

#endif

