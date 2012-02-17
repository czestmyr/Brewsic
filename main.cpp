#include <iostream>
#include <SDL.h>
#include <cmath>
#include <climits>
#include <fstream>

#include "mixer/mixer.h"
#include "generators/sine.h"
#include "generators/saw.h"
#include "generators/square.h"
#include "filters/gate.h"
#include "filters/adsr.h"
#include "filters/delay.h"
#include "filters/flanger.h"
#include "filters/volume.h"
#include "synths/triosc.h"
#include "synths/synthfactory.h"
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
#include "gui/filters/delaygui.h"
#include "gui/synths/triosc.h"

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

SynthFactory synthFactory(_SAMPLES);
SafePtr<ISynth> osc;

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

Slider* s1;
Checkbox* ch1;

class DGCreator: public IObserver {
	public:
		DGCreator(SafePtr<IControl> parent): _parent(parent), _dg((IControl*)NULL) {
			_prop.addObserver(this);
		}

		void signal() {
			_dg = safe_new(DelayGui(_parent, &delay));
		}

		Property<int> _prop;
	private:
		SafePtr<IControl> _parent;
		SafePtr<IControl> _dg;
};

class TGCreator: public IObserver {
	public:
		TGCreator(SafePtr<IControl> parent): _parent(parent), _tg((IControl*)NULL) {
			_prop.addObserver(this);
		}

		void signal() {
			if (_tg) _tg->deleteMe();
			_tg = safe_new(TripleOscillatorGui(_parent, (TripleOscillator*)osc.get()));
		}

		Property<int> _prop;
	private:
		SafePtr<IControl> _parent;
		SafePtr<IControl> _tg;
};

class KeyboardMover: public IObserver {
	public:
		KeyboardMover(SafePtr<Keyboard> kbd, bool up): _kbd(kbd), _up(up) {
			_prop.addObserver(this);
		}

		void signal() {
			if (_up) {
				int shift = _kbd->getShift() - 20;
				if (shift < 0) shift = 0;
				_kbd->setShift(shift);
			} else {
				int shift = _kbd->getShift() + 20;
				_kbd->setShift(shift);
			}
		}

		Property<int> _prop;
	private:
		SafePtr<Keyboard> _kbd;
		bool _up;
};

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

	// Synth factory test
	osc = synthFactory.createNewSynth("TripleOscillator");

	SDL_PauseAudio(0);

	// Gui setup
	GuiMgr gui;
	SafePtr<IControl> gui_bg = safe_new(Background((IControl*)NULL, 4, 4, WIDTH-8, HEIGHT-8));
	gui.adoptControl(gui_bg);

	//Quit button
	Property<int> quit_prop(0);
	new Button(gui_bg, WIDTH, 5, "Quit Brewsic", &quit_prop);

	SafePtr<Keyboard> kbd = safe_new(Keyboard(gui_bg, WIDTH - 200, 50, 400)).cast<Keyboard>();
	kbd->setSynth(&osc);
	KeyboardMover kbd_up(kbd, true);
	KeyboardMover kbd_dn(kbd, false);

	new Button(gui_bg, WIDTH - 250, 50, "Up", &kbd_up._prop);
	new Button(gui_bg, WIDTH - 250, 75, "Down", &kbd_dn._prop);

	SafePtr<PictureSelector> psel1 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 100, 32, 32, &osc._first_gen)).cast<PictureSelector>();
	SafePtr<PictureSelector> psel2 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 140, 32, 32, &osc._second_gen)).cast<PictureSelector>();
	SafePtr<PictureSelector> psel3 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 180, 32, 32, &osc._third_gen)).cast<PictureSelector>();
	for (int i = 0; i < (int)GEN_NUMBER; ++i) {
		const char* filename = GeneratorFactory::inst()->getGeneratorPictureFilename((GeneratorType)i);
		psel1->addPicture(filename);
		psel2->addPicture(filename);
		psel3->addPicture(filename);
	}

	//Sequencer
	int seq_size = 8;
	int seq_length = 500;
	int seq_ind = 0;
	int seq_dur = 0;
	Property<bool> sequencer[seq_size];
	Property<float> seq_freq[seq_size];
	for (int i = 0; i < seq_size; ++i) {
		safe_new(Checkbox(gui_bg, 50 + i*30, 50, 20, 20, &sequencer[i]));
		safe_new(Slider(gui_bg, 50 + i*30, 75, 200, 100.0, 2000.0, &seq_freq[i]));
	}

	//Timing
	Uint32 time = SDL_GetTicks();
	Uint32 dt = 0;

	// Delay gui test
	SafePtr<Button> dgCreate = safe_new(Button(gui_bg, 10, 290, "Create delay filter window")).cast<Button>();

	// Triosc gui test
	TGCreator tgCreator(gui_bg);
	SafePtr<Button> tgCreate = safe_new(Button(gui_bg, 10, 310, "Create 3xOsc GUI", &tgCreator._prop)).cast<Button>();

	// Main event loop
	SDL_Event e;
	while (!quit_prop) {
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
					quit_prop = 1;
				break;
			}
		}

		// Timing
		dt = SDL_GetTicks() - time;
		time += dt;

		// Sequencer
		seq_dur += dt;
		while (seq_dur >= seq_length) {
			osc->stopNote(seq_ind+1);
			seq_dur -= seq_length;
			seq_ind++;
			if (seq_ind >= seq_size) seq_ind %= seq_size;
			if (sequencer[seq_ind]) {
				osc->startNote(seq_ind+1, seq_freq[seq_ind]);
			}
		}

		SDL_Color shadecol = Style::inst()->getShadeColor();
		Uint32 shade = SDL_MapRGB(screen->format, shadecol.r, shadecol.g, shadecol.b);
		SDL_FillRect(screen, NULL, shade);
		gui.cleanup();
		gui.draw(screen);
		SDL_Flip(screen);
	}

	SDL_CloseAudio();
	free(obtained);
	SDL_Quit();

	return 0;
}

