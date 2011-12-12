#ifndef _INTERFACE_SYNTH_H__
#define _INTERFACE_SYNTH_H__

class ISynth {
	public:
		virtual void startNote(int noteId, float frequency) = 0;
		virtual void stopNote(int noteId) = 0;

		virtual void generateOutput(float* buffer) = 0;
};

#endif

