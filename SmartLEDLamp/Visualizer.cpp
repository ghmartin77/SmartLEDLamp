#include "Visualizer.h"
#include <EEPROM.h>
#include "Log.h"

Visualizer::Visualizer() :
		speed(0), speedCounter(0) {
	memset(imageData, 0, sizeof(imageData) / sizeof(imageData[0]));
}

Visualizer::~Visualizer() {
}

uint8_t* Visualizer::renderNextImage() {
	if (speedCounter == 0) {
		computeImage();
	}

	if (speedCounter++ >= speed) {
		speedCounter = 0;
	}

	return imageData;
}

void Visualizer::readRuntimeConfiguration(int &address) {
	Logger.debug("Visualizer::readRuntimeConfiguration");
	EEPROM.get(address, speed);
	address += sizeof(speed);
}

void Visualizer::writeRuntimeConfiguration(int &address) {
	Logger.debug("Visualizer::writeRuntimeConfiguration");
	EEPROM.write(address, speed);
	address += sizeof(speed);
}
