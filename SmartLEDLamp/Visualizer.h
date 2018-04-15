#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include <Arduino.h>
#include "defines.h"

class Visualizer {
public:
	Visualizer();
	virtual ~Visualizer();

	virtual void start() {
	}

	virtual void stop() {
	}

	uint8_t* renderNextImage();

	virtual boolean onButtonPressed(uint8_t button) {
		return false;
	}

protected:
	int8_t speed;
	int8_t speedCounter;

	uint8_t imageData[LEDS_WIDTH * LEDS_HEIGHT * 3];

	virtual void computeImage() = 0;
};

#endif /* VISUALIZER_H_ */
