#include <iostream>
#include <SDL.h>
#include <cmath>
#include <climits>
#include <fstream>

#include "mixer/mixer.h"
#include "mixer/mainmixer.h"
#include "mixer/synthqueue.h"
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
#include "gui/matrix.h"
#include "gui/pselect.h"
#include "gui/image.h"
#include "gui/wheel.h"
#include "gui/slider.h"
#include "gui/checkbox.h"
#include "gui/style.h"
#include "gui/filters/delaygui.h"
#include "gui/synths/triosc.h"
#include "common/signals.h"

using namespace std;

#define _FREQ 22050
#define _SAMPLES 256
#define _CHANNELS 16

#define WIDTH 800
#define HEIGHT 600

MainMixer mmix(_SAMPLES, _CHANNELS);
SynthFactory synthFactory(_SAMPLES);
SafePtr<TripleOscillator> osc;

ofstream ofile;

void audioCallback(void *userdata, Uint8 *stream, int len) {
	mmix.mixInto((Uint16*)stream);
}

bool do_quit(false);
SafePtr<Keyboard> kbd;
SafePtr<Matrix> mtrx;

class MainSignals {
	public:
		MainSignals(): _quit(this), _kbdUp(this), _kbdDown(this) {}

		SIGNAL_DESTINATION(_quit, MainSignals, quit);
		SIGNAL_DESTINATION(_kbdUp, MainSignals, kbdUp);
		SIGNAL_DESTINATION(_kbdDown, MainSignals, kbdDown);
	
		void quit() { do_quit = true; }
		void kbdUp() {
			int shift = kbd->getShift() - 20;
			if (shift < 0) shift = 0;
			kbd->setShift(shift);
			mtrx->setShift(shift);
		}
		void kbdDown() {
			int shift = kbd->getShift() + 20;
			kbd->setShift(shift);
			mtrx->setShift(shift);
		}
};
MainSignals msig;

int main(int argc, char* argv[]) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		cerr << "Could not initialize SDL!" << endl;
		return -1;
	}

	// TODO: Move audio initialization elsewhere
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

	// Gui setup
	GuiMgr* gui = new GuiMgr();
	SafePtr<IControl> gui_bg = safe_new(Background((IControl*)NULL, 4, 4, WIDTH-8, HEIGHT-8));
	gui->adoptControl(gui_bg);

	// Synth factory test
	osc = synthFactory.createNewSynth("TripleOscillator").cast<TripleOscillator>();
	osc->setGuiParent(gui_bg);

	// Mixer
	SynthQueue* sq = mmix.getSynthQueue(0);
	sq->setGuiParent(gui_bg);
	sq->pushSynth(osc.cast<ISynth>());

	safe_new(Button(gui_bg, 600, 500, "Synth Queue 1 Gui", sq->_guiSignal.getSignal()));

	// Quit button
	new Button(gui_bg, WIDTH, 5, "Quit Brewsic", msig._quit.getSignal());

	// Keyboard and matrix test
	kbd = safe_new(Keyboard(gui_bg, 50, 350, 200)).cast<Keyboard>();
	kbd->setSynth(osc.cast<ISynth>());
	mtrx = safe_new(Matrix(gui_bg, 130, 350, 400, 200)).cast<Matrix>();

	new Button(gui_bg, WIDTH - 250, 50, "Up", msig._kbdUp.getSignal());
	new Button(gui_bg, WIDTH - 250, 75, "Down", msig._kbdDown.getSignal());

	// Picture selectors for triosc
	SafePtr<PictureSelector> psel1 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 100, 32, 32, &osc->_first_gen)).cast<PictureSelector>();
	SafePtr<PictureSelector> psel2 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 140, 32, 32, &osc->_second_gen)).cast<PictureSelector>();
	SafePtr<PictureSelector> psel3 = safe_new(PictureSelector(gui_bg, WIDTH - 250, 180, 32, 32, &osc->_third_gen)).cast<PictureSelector>();
	for (int i = 0; i < (int)GEN_NUMBER; ++i) {
		const char* filename = GeneratorFactory::inst()->getGeneratorPictureFilename((GeneratorType)i);
		psel1->addPicture(filename);
		psel2->addPicture(filename);
		psel3->addPicture(filename);
	}

	// Sequencer
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

	// Timing
	Uint32 time = SDL_GetTicks();
	Uint32 dt = 0;

	// Start sound
	SDL_PauseAudio(0);

	// Main event loop
	SDL_Event e;
	while (!do_quit) {
		SDL_Delay(50);

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_LEFT) {
						gui->leftPress(e.button.x, e.button.y);
					} else if (e.button.button == SDL_BUTTON_RIGHT) {
						gui->rightPress(e.button.x, e.button.y);
					}
				break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT) {
						gui->leftRelease(e.button.x, e.button.y);
					} else if (e.button.button == SDL_BUTTON_RIGHT) {
						gui->rightRelease(e.button.x, e.button.y);
					}
				break;
				case SDL_MOUSEMOTION:
					gui->mouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
				break;
				case SDL_KEYDOWN:
					gui->keyPress(e.key.keysym.sym);
				break;
				case SDL_KEYUP:
					gui->keyRelease(e.key.keysym.sym);
				break;
				case SDL_QUIT:
					do_quit = true;
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
		gui->cleanup();
		gui->draw(screen);
		SDL_Flip(screen);
	}

	SDL_CloseAudio();
	free(obtained);
	SDL_Quit();

	delete gui;
	std::cout << "Undeleted control number: " << IControl::ctlCounter << std::endl;

	return 0;
}

