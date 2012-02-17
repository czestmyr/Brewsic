#ifndef _MIXER_H_
#define _MIXER_H_

#include <SDL.h>

class Mixer {
	public:
		Mixer(int bufsize) {
			_buffer = new float[bufsize];
			_bufsize = bufsize;
		}

		~Mixer() {
			delete[] _buffer;
		}

		void clear();
		void mixIn(float* buffer);
		void copyBuffer(Uint16* buffer);
		void copyBufferFloat(float* buffer);

		float* getBuffer() { return _buffer; }

	private:
		float* _buffer;
		int _bufsize;
};

#endif

