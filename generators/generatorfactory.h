#ifndef _GENERATOR_FACTORY_H_
#define _GENERATOR_FACTORY_H_

class IGenerator;

enum GeneratorType {
	GEN_SINE = 0,
	GEN_SAW = 1,
	GEN_SQUARE = 2,
	GEN_NUMBER = 3
};

class GeneratorFactory {
	public:
		IGenerator* createGenerator(GeneratorType type, float frequency, float phase);
		const char* getGeneratorPictureFilename(GeneratorType type);

		static GeneratorFactory* inst() {
			if (!_inst) {
				_inst = new GeneratorFactory();
			}
			return _inst;
		}
	private:
		static GeneratorFactory* _inst;
};

#endif

