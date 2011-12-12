#include <iostream>
#include <SDL.h>
#include <cmath>
#include <climits>
#include <fstream>

#include "mixer.h"
#include "generators/sine.h"
#include "generators/saw.h"
#include "generators/square.h"
#include "filters/gate.h"
#include "filters/adsr.h"
#include "filters/delay.h"
#include "filters/flanger.h"
#include "filters/volume.h"
#include "synths/triosc.h"
#include "gui/guimgr.h"
#include "gui/window.h"
#include "gui/background.h"

using namespace std;

#define _FREQ 22050
#define _SAMPLES 256

#define WIDTH 800
#define HEIGHT 600

SquareGenerator ga(20);
SineGenerator gb(80);
SineGenerator gc(70);
GateFilter gate(10.0, 0.1);
Adsr adsr(1, 5, 10, 150);
DelayFilter delay(5000, 0.80);
DelayFilter delay2(10000, 0.90);
FlangerFilter flanger(54);
FlangerFilter flanger2(100);
FlangerFilter flanger3(83);
FlangerFilter flanger4(200);
Volume vol1(8.0);
Volume vol2(0.5);
float a[_SAMPLES];
float b[_SAMPLES];
float c[_SAMPLES];
Uint16 buf[_SAMPLES];
Mixer mix(_SAMPLES);

TripleOscillator osc(_SAMPLES);

float myMin = 20;
float myMax = 20000;
float myValue = 440;
float myMin1 = -2400;
float myMax1 = 2400;
float myValue1 = -30;
float myMin2 = -2400;
float myMax2 = 2400;
float myValue2 = 30;

ofstream ofile;

void audioCallback(void *userdata, Uint8 *stream, int len) {
	mix.clear();
	osc.generateOutput(mix.getBuffer());
	delay.filter(_SAMPLES, mix.getBuffer());
	delay2.filter(_SAMPLES, mix.getBuffer());
	flanger.filter(_SAMPLES, mix.getBuffer());
	flanger2.filter(_SAMPLES, mix.getBuffer());
	flanger3.filter(_SAMPLES, mix.getBuffer());
	flanger4.filter(_SAMPLES, mix.getBuffer());
	vol2.filter(_SAMPLES, mix.getBuffer());
//	gate.filter(_SAMPLES, mix.getBuffer());

	ga.generate(_SAMPLES, a);
	adsr.filter(_SAMPLES, a);
	vol1.filter(_SAMPLES, a);
	mix.mixIn(a);

	mix.copyBuffer((Uint16*)stream);

//Test: output into a raw file
//	for (int i = 0; i < len; ++i) {
//		ofile << mix.getBuffer()[i];
//	}
}

int main(int argc, char* argv[]) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		cerr << "Could not initialize SDL!" << endl;
		return -1;
	}

	SDL_AudioSpec* desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
	SDL_AudioSpec* obtained = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

	desired->freq = _FREQ;
	desired->format = AUDIO_S16SYS;
	desired->channels = 1;
	desired->samples = _SAMPLES*2;
	desired->callback = audioCallback;
	desired->userdata = NULL;

	if (SDL_OpenAudio(desired, obtained)) {
		cerr << "Could not open audio device!" << endl;
		SDL_Quit();
		return -1;
	}

	cout << "Frequency : " << obtained->freq << endl;
	cout << "Channels  : " << obtained->channels << endl;
	cout << "Samples   : " << obtained->samples << endl;

	free(desired);

	SDL_Surface* screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, 0);

	SDL_PauseAudio(0);

	// Gui setup
	GuiMgr gui;
	IControl* gui_bg = new Background(NULL, 4, 4, WIDTH-8, HEIGHT-8);
	gui.adoptControl(gui_bg);
	new Window(gui_bg, 10, 10, 500, 300);

	// Main event loop
	SDL_Event e;
	bool cont = true;
	while (cont) {
		SDL_Delay(50);

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_LEFT) {
						gui.leftPress(e.button.x, e.button.y);
					} else if (e.button.button == SDL_BUTTON_RIGHT) {
						gui.rightPress(e.button.x, e.button.y);
					}
				break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT) {
						gui.leftRelease(e.button.x, e.button.y);
					} else if (e.button.button == SDL_BUTTON_RIGHT) {
						gui.rightRelease(e.button.x, e.button.y);
					}
				break;
				case SDL_MOUSEMOTION:
					gui.mouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
				break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							cont = false;
						break;
					}
				break;
				case SDL_QUIT:
					cont = false;
				break;
			}
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		gui.draw(screen);
		SDL_Flip(screen);
	}

//	ofile.close();

	SDL_CloseAudio();
	free(obtained);
	SDL_Quit();

	return 0;
}

