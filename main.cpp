#include <SDL.h>

#include "maintest.h"
#include "gui/Icontrol.h"
#include "common/propertytest.h"

using namespace std;

int main(int argc, char* argv[]) {
  //PropertyTest::runTest();
  //return 0;

  MainTest test;

  int initResult = test.init();
  if (initResult) return initResult;
  test.mainLoop();
  test.deinit();

  IControl::dumpRegisteredControls();

  return 0;
}

