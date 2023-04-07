#include "App.h"

App::App(uint8_t appId, LEDMatrix* pLEDMatrix) :
		id(appId), pMatrix(pLEDMatrix), name("Unknown") {

}

App::~App() {

}
