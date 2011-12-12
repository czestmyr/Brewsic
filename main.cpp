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

using namespace std;

#define _FREQ 22050
#define _SAMPLES 256

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

static void ChangeFreq (AG_Event* event) {
	AG_Slider* sl = (AG_Slider*)AG_SELF();

	switch (AG_GetInt(sl, "sliderID")) {
		case 0:
		default:
			osc.setBase(AG_GetFloat(sl, "value"));
			break;
		case 1:
			osc.setFirst(AG_GetFloat(sl, "value"));
			break;
		case 2:
			osc.setSecond(AG_GetFloat(sl, "value"));
			break;
	}

//	IGenerator* gen = (IGenerator*)AG_GetPointer(sl, "gen");
//	gen->setFreq(AG_GetFloat(sl, "value"));
}

static void TriggerAdsr (AG_Event* event) {
	AG_Button* bt = (AG_Button*)AG_SELF();

	float freq = AG_GetFloat(bt, "freq");
	ga.setFreq(myValue*freq);
	gb.setFreq(myValue2*freq);
	gc.setFreq(myValue1*freq);
	adsr.trigger();
	adsr.release();
}

static void KeyPress (AG_Event* event) {
	float freq = 55.0;

	switch(AG_INT(1)){
		//First octave
		case AG_KEY_Z: freq = freq; break;
		case AG_KEY_S: freq = freq * pow(2,1.0/12); break;
		case AG_KEY_X: freq = freq * pow(2,2.0/12); break;
		case AG_KEY_D: freq = freq * pow(2,3.0/12); break;
		case AG_KEY_C: freq = freq * pow(2,4.0/12); break;
		case AG_KEY_V: freq = freq * pow(2,5.0/12); break;
		case AG_KEY_G: freq = freq * pow(2,6.0/12); break;
		case AG_KEY_B: freq = freq * pow(2,7.0/12); break;
		case AG_KEY_H: freq = freq * pow(2,8.0/12); break;
		case AG_KEY_N: freq = freq * pow(2,9.0/12); break;
		case AG_KEY_J: freq = freq * pow(2,10.0/12); break;
		case AG_KEY_M: freq = freq * pow(2,11.0/12); break;

		//Second octave
		case AG_KEY_COMMA: freq = freq * pow(2,12.0/12); break;
		case AG_KEY_L: freq = freq * pow(2,13.0/12); break;
		case AG_KEY_PERIOD: freq = freq * pow(2,14.0/12); break;
		case AG_KEY_SEMICOLON: freq = freq * pow(2,15.0/12); break;
		case AG_KEY_SLASH: freq = freq * pow(2,16.0/12); break;
		case AG_KEY_Q: freq = freq * pow(2,17.0/12); break;
		case AG_KEY_2: freq = freq * pow(2,18.0/12); break;
		case AG_KEY_W: freq = freq * pow(2,19.0/12); break;
		case AG_KEY_3: freq = freq * pow(2,20.0/12); break;
		case AG_KEY_E: freq = freq * pow(2,21.0/12); break;
		case AG_KEY_4: freq = freq * pow(2,22.0/12); break;
		case AG_KEY_R: freq = freq * pow(2,23.0/12); break;

		//Third octave
		case AG_KEY_T: freq = freq * pow(2,24.0/12); break;
		case AG_KEY_6: freq = freq * pow(2,25.0/12); break;
		case AG_KEY_Y: freq = freq * pow(2,26.0/12); break;
		case AG_KEY_7: freq = freq * pow(2,27.0/12); break;
		case AG_KEY_U: freq = freq * pow(2,28.0/12); break;
		case AG_KEY_I: freq = freq * pow(2,29.0/12); break;
		case AG_KEY_9: freq = freq * pow(2,30.0/12); break;
		case AG_KEY_O: freq = freq * pow(2,31.0/12); break;
		case AG_KEY_0: freq = freq * pow(2,32.0/12); break;
		case AG_KEY_P: freq = freq * pow(2,33.0/12); break;
		case AG_KEY_MINUS: freq = freq * pow(2,34.0/12); break;
		case AG_KEY_LEFTBRACKET: freq = freq * pow(2,35.0/12); break;
	}

//	ga.setFreq(myValue*freq);
//	gb.setFreq(myValue2*freq);
//	gc.setFreq(myValue1*freq);
	if (AG_INT(1) == AG_KEY_SPACE) {
		adsr.trigger();
		adsr.release();
	} else {
		osc.startNote(AG_INT(1), freq);
	}
}

static void KeyRelease (AG_Event* event) {
//	adsr.release();
	osc.stopNote(AG_INT(1));
}

ofstream ofile;

void audioCallback(void *userdata, Uint8 *stream, int len) {
/*	gb.generate(_SAMPLES, b);
	gc.generate(_SAMPLES, c);
	mix.clear();
	mix.mixIn(a);
	mix.mixIn(b);
	mix.mixIn(c);
	gate.filter(_SAMPLES, mix.getBuffer());
	adsr.filter(_SAMPLES, mix.getBuffer());
	delay.filter(_SAMPLES, mix.getBuffer());
	delay2.filter(_SAMPLES, mix.getBuffer());*/

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

	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, 0);

	SDL_PauseAudio(0);

	AG_InitCore("myapp", 0);
	AG_InitVideoSDL(screen, 0);

	AG_Window* win = AG_WindowNew(0);
	AG_WindowSetGeometry(win, -1, -1, 640, 480);

	AG_Box* box = AG_BoxNew(win, AG_BOX_HORIZ, AG_BOX_HFILL|AG_BOX_VFILL);

	//Test: keyboard events
	AG_Button* play_btn = AG_ButtonNew(box, 0, "");
	AG_WidgetSetSize(play_btn, 300, 300);
	AG_SetEvent(play_btn, "key-down", KeyPress, NULL);
	AG_SetEvent(play_btn, "key-up", KeyRelease, NULL);

	AG_Slider *sliderA;
	sliderA = AG_SliderNewFlt(box, AG_SLIDER_VERT, AG_SLIDER_VFILL, &myValue, &myMin, &myMax);
	AG_WidgetSetSize(sliderA, 10, 500);
	AG_WidgetSetPosition(sliderA, 10, 10);
	AG_SetPointer(sliderA, "gen", (void*)&ga);
	AG_SetInt(sliderA, "sliderID", 0);
	AG_SetEvent(sliderA, "slider-changed", ChangeFreq, "");

	AG_Slider *sliderB;
	sliderB = AG_SliderNewFlt(box, AG_SLIDER_VERT, AG_SLIDER_VFILL, &myValue1, &myMin1, &myMax1);
	AG_WidgetSetSize(sliderB, 10, 500);
	AG_WidgetSetPosition(sliderB, 110, 10);
	AG_SetPointer(sliderB, "gen", (void*)&gb);
	AG_SetInt(sliderB, "sliderID", 1);
	AG_SetEvent(sliderB, "slider-changed", ChangeFreq, "");

	AG_Slider *sliderC;
	sliderC = AG_SliderNewFlt(box, AG_SLIDER_VERT, AG_SLIDER_VFILL, &myValue2, &myMin2, &myMax2);
	AG_WidgetSetSize(sliderC, 10, 500);
	AG_WidgetSetPosition(sliderC, 210, 10);
	AG_SetPointer(sliderC, "gen", (void*)&gc);
	AG_SetInt(sliderC, "sliderID", 2);
	AG_SetEvent(sliderC, "slider-changed", ChangeFreq, "");

/*	for (int f = 220.0; f < 1000; f *= pow(2,1.0/12)) {
		AG_Button* btn1;
		btn1 = AG_ButtonNew(box, 0, "");
		AG_WidgetSetSize(btn1, 10, 100);
		AG_SetEvent(btn1, "button-pushed", TriggerAdsr, "");
		AG_SetFloat(btn1, "freq", f);
	}*/

//	ofile.open("output.raw");

	AG_WindowShow(win);

	AG_EventLoop();

	// Main event loop
/*	SDL_Event e;
	bool cont = true;
	while (cont) {
		SDL_Delay(50);

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
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
	}*/

//	ofile.close();

	SDL_CloseAudio();
	free(obtained);
	SDL_Quit();

	return 0;
}

