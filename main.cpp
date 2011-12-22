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
#include "gui/button.h"
#include "gui/style.h"
#include "gui/keyboard.h"
#include "gui/pselect.h"
#include "gui/image.h"
#include "gui/wheel.h"
#include "gui/slider.h"
#include "gui/checkbox.h"
#include "gui/style.h"

#include "common/propertytest.h"

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
//	flanger.filter(_SAMPLES, mix.getBuffer());
//	flanger2.filter(_SAMPLES, mix.getBuffer());
//	flanger3.filter(_SAMPLES, mix.getBuffer());
//	flanger4.filter(_SAMPLES, mix.getBuffer());
	vol2.filter(_SAMPLES, mix.getBuffer());
//	gate.filter(_SAMPLES, mix.getBuffer());

	ga.generate(_SAMPLES, a);
	adsr.filter(_SAMPLES, a);
	vol1.filter(_SAMPLES, a);
	mix.mixIn(a);

	mix.copyBuffer((Uint16*)stream);
}

bool g_quit = false;

void exitCallback(void* data) {
	g_quit = true;
}

void kbd_up(void* data) {
	Keyboard* kbd = (Keyboard*)data;
	int shift = kbd->getShift() - 20;
	if (shift < 0) shift = 0;
	kbd->setShift(shift);
}

void kbd_dn(void* data) {
	Keyboard* kbd = (Keyboard*)data;
	int shift = kbd->getShift() + 20;
	kbd->setShift(shift);
}

PictureSelector* psel1;
PictureSelector* psel2;
PictureSelector* psel3;

void osc1Callback(void* data) {
	switch (psel1->getSelection()) {
		case 0:
			osc.setGenerator(0, new SineGenerator(440));
		break;
		case 1:
			osc.setGenerator(0, new SawGenerator(440));
		break;
		case 2:
			osc.setGenerator(0, new SquareGenerator(440));
		break;
	}
}

void osc2Callback(void* data) {
	switch (psel2->getSelection()) {
		case 0:
			osc.setGenerator(1, new SineGenerator(440));
		break;
		case 1:
			osc.setGenerator(1, new SawGenerator(440));
		break;
		case 2:
			osc.setGenerator(1, new SquareGenerator(440));
		break;
	}
}

void osc3Callback(void* data) {
	switch (psel3->getSelection()) {
		case 0:
			osc.setGenerator(2, new SineGenerator(440));
		break;
		case 1:
			osc.setGenerator(2, new SawGenerator(440));
		break;
		case 2:
			osc.setGenerator(2, new SquareGenerator(440));
		break;
	}
}

Wheel* w1;
Wheel* w2;
Slider* s1;
Checkbox* ch1;

void wheel1Callback(void* data) {
	osc.setFirst(w1->getValue());
}

void wheel2Callback(void* data) {
	osc.setSecond(w2->getValue());
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
	//IControl* gui_outer = new Window(gui_bg, 10, 10, 500, 300, "Outer window");
	//IControl* gui_inner = new Window(gui_outer, 10, 10, 300, 200, "Inner window");

	Button* gui_btn = new Button(gui_bg, WIDTH, 5, "Quit Brewsic");
	gui_btn->setCallback(exitCallback);

	Keyboard* kbd = new Keyboard(gui_bg, WIDTH - 200, 50, 400);
	kbd->setSynth(&osc);

	gui_btn = new Button(gui_bg, WIDTH - 250, 50, "Up", kbd);
	gui_btn->setCallback(kbd_up);
	gui_btn = new Button(gui_bg, WIDTH - 250, 75, "Down", kbd);
	gui_btn->setCallback(kbd_dn);

	psel1 = new PictureSelector(gui_bg, WIDTH - 250, 100, 32, 32);
	psel1->addPicture("data/images/sine.png");
	psel1->addPicture("data/images/saw.png");
	psel1->addPicture("data/images/square.png");
	psel1->setCallback(osc1Callback);

	psel2 = new PictureSelector(gui_bg, WIDTH - 250, 140, 32, 32);
	psel2->addPicture("data/images/sine.png");
	psel2->addPicture("data/images/saw.png");
	psel2->addPicture("data/images/square.png");
	psel2->setCallback(osc2Callback);

	psel3 = new PictureSelector(gui_bg, WIDTH - 250, 180, 32, 32);
	psel3->addPicture("data/images/sine.png");
	psel3->addPicture("data/images/saw.png");
	psel3->addPicture("data/images/square.png");
	psel3->setCallback(osc3Callback);

	w1 = new Wheel(gui_bg, WIDTH - 250, 230, 20, 20, -300, 300, &osc._first);
	w2 = new Wheel(gui_bg, WIDTH - 275, 230, 20, 20, -100, 100, &osc._second);

	s1 = new Slider(gui_bg, WIDTH - 300, 230, 100, -100, 100, &osc._shift);

	ch1 = new Checkbox(gui_bg, WIDTH - 275, 255, 20, 20, NULL);

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
					gui.keyPress(e.key.keysym.sym);
				break;
				case SDL_KEYUP:
					gui.keyRelease(e.key.keysym.sym);
				break;
				case SDL_QUIT:
					cont = false;
				break;
			}
		}

		if (g_quit) cont = false;

		SDL_Color shadecol = Style::inst()->getShadeColor();
		Uint32 shade = SDL_MapRGB(screen->format, shadecol.r, shadecol.g, shadecol.b);
		SDL_FillRect(screen, NULL, shade);
		gui.draw(screen);
		SDL_Flip(screen);
	}

	SDL_CloseAudio();
	free(obtained);
	SDL_Quit();

	return 0;
}

