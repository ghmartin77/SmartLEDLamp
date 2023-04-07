#ifndef DEMOREELVISUALIZER_H_
#define DEMOREELVISUALIZER_H_

#define FASTLED_INTERNAL

#include "Visualizer.h"
#include "FastLED.h"
#include "defines.h"

class DemoReelVisualizer: public Visualizer {
public:
	DemoReelVisualizer();
	virtual ~DemoReelVisualizer();

	virtual void start();
	virtual void stop();

	virtual boolean onButtonPressed(uint8_t button);

	virtual void readRuntimeConfiguration(int &address);
	virtual void writeRuntimeConfiguration(int &address);

protected:
	virtual void computeImage();

private:
	int8_t currentPatternNumber = 0;
	uint8_t hue = 0;
	int lastPos = -1;

	CRGB leds[LEDS_HEIGHT];

	void rainbow();
	void confetti();
	void sinelon();
	void bpm();
	void juggle();
};

#endif /* DEMOREELVISUALIZER_H_ */
