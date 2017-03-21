#include "Visualizer.h"

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
