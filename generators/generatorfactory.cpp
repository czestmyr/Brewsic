#include "generatorfactory.h"
#include "Igenerator.h"
#include "sine.h"
#include "saw.h"
#include "square.h"

GeneratorFactory* GeneratorFactory::_inst = NULL;

IGenerator* GeneratorFactory::createGenerator(GeneratorType type, float frequency, float phase) {
	switch (type) {
		case GEN_SINE:
			return new SineGenerator(frequency, phase);
		break;
		case GEN_SAW:
			return new SawGenerator(frequency, phase);
		break;
		case GEN_SQUARE:
			return new SquareGenerator(frequency, phase);
		break;
	}
}

const char* GeneratorFactory::getGeneratorPictureFilename(GeneratorType type) {
	switch (type) {
		case GEN_SINE:
			return "data/images/sine.png";
		break;
		case GEN_SAW:
			return "data/images/saw.png";
		break;
		case GEN_SQUARE:
			return "data/images/square.png";
		break;
	}
}

