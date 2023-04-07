#ifndef NOISEWITHPALETTEVISUALIZER_H_
#define NOISEWITHPALETTEVISUALIZER_H_

#define FASTLED_INTERNAL

#include "Visualizer.h"
#include "FastLED.h"
#include "defines.h"

class NoiseWithPaletteVisualizer: public Visualizer {
public:
	NoiseWithPaletteVisualizer();
	virtual ~NoiseWithPaletteVisualizer();

	virtual void start();
	virtual void stop();

	virtual boolean onButtonPressed(uint8_t button);

	virtual void readRuntimeConfiguration(int &address);
	virtual void writeRuntimeConfiguration(int &address);

protected:
	virtual void computeImage();

	void fillnoise8();
	void mapNoiseToLEDsUsingPalette();
	void applyProfile();
	void SetupRandomPalette();
	void SetupBlackAndWhiteStripedPalette();
	void SetupPurpleAndGreenPalette();

private:
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;

	uint8_t profile = 1;

	uint8_t noise[LEDS_HEIGHT][LEDS_HEIGHT];
	uint16_t speed = 1;
	uint16_t scale = 15;
	CRGBPalette16 currentPalette;
	uint8_t colorLoop = 1;
	uint8_t ihue = 0;
};

#endif /* NOISEWITHPALETTEVISUALIZER_H_ */
