#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include <Arduino.h>
#include "defines.h"
#include "RuntimeConfigurable.h"

class Visualizer: public virtual RuntimeConfigurable {
public:
	Visualizer();
	virtual ~Visualizer();

	virtual void start() {
	}

	virtual void stop() {
	}

	uint8_t* renderNextImage();

	uint8_t* getImage() {
		return imageData;
	}

	virtual boolean onButtonPressed(uint8_t button) {
		return false;
	}

	virtual void readRuntimeConfiguration(int &address);
	virtual void writeRuntimeConfiguration(int &address);

protected:
	int8_t speed;
	int8_t speedCounter;

	uint8_t imageData[LEDS_WIDTH * LEDS_HEIGHT * 3];

	virtual void computeImage() = 0;
};

#endif /* VISUALIZER_H_ */
