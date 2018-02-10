#ifndef VUMETERVISUALIZER_H_
#define VUMETERVISUALIZER_H_

#define FASTLED_INTERNAL

#include "Visualizer.h"
#include "FastLED.h"
#include "defines.h"

class VUMeterVisualizer: public Visualizer {
public:
	VUMeterVisualizer(float* pAmp);
	virtual ~VUMeterVisualizer();

	virtual void start();
	virtual void stop();

	virtual boolean onButtonPressed(uint8_t button);

protected:
	virtual void computeImage();
	void computeImage1();
	void computeImage2();
	void computeImage3();

private:
	int8_t rendererId = 0;
	int8_t paletteNo;
	int16_t paletteMax;
	const TProgmemRGBPalette16 VUMeter_p FL_PROGMEM = { CRGB::Green,
			CRGB::Green, CRGB::Green, CRGB::Green,

			CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green,

			CRGB::Green, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow,

			CRGB::Yellow, CRGB::Red, CRGB::Red, CRGB::Red };
	CRGBPalette16 palette[6] = { CRGBPalette16(HeatColors_p), CRGBPalette16(
			PartyColors_p), CRGBPalette16(CloudColors_p), CRGBPalette16(
			LavaColors_p), CRGBPalette16(VUMeter_p), CRGBPalette16(
			OceanColors_p) };
	byte heat[LEDS_WIDTH][LEDS_HEIGHT * 2];

	int16_t cooling;
	int16_t sparking;

	uint8_t vImageData[LEDS_WIDTH * LEDS_HEIGHT * 2 * 3];

	float* pAmplitude;

};

#endif /* VUMETERVISUALIZER_H_ */
