#include "synths/Isynth.h"
#include <sstream>

using namespace std;

void ISynth::constructSynthName() {
  _lastSerialNumber++;
  stringstream name;
  name << this->getClassName() << " " << _lastSerialNumber;
  _synth_name = name.str();
}

int ISynth::_lastSerialNumber = 0;

