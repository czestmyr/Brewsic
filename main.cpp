//#include <SDL.h>

//#include "maintest.h"
//#include "gui/Icontrol.h"
#include "common/propertytest.h"

using namespace std;

//MainTest test;

int main(int argc, char* argv[]) {
/*  int initResult = test.init();
  if (initResult) return initResult;
  test.mainLoop();
  test.deinit();

  IControl::dumpRegisteredControls();*/
  PropertyTest::runTest();

  return 0;
}

