#include "VUMeterVisualizer.h"

VUMeterVisualizer::VUMeterVisualizer(float *pAmp) :
		pAmplitude(pAmp), cooling(95), sparking(200), paletteNo(6), paletteMax(
				240) {
}

VUMeterVisualizer::~VUMeterVisualizer() {
}

void VUMeterVisualizer::computeImage() {
	int leds = LEDS_HEIGHT;

	int amp = *pAmplitude * 200;

	if (amp > LEDS_HEIGHT)
		amp = LEDS_HEIGHT;

	static char* pC =
			"###################################################################################";

	pC[amp] = 0;
	Serial.println(pC);
	pC[amp] = '#';

//	Serial.println(amp);

	for (int j = 0; j < leds; j++) {
		byte colorindex = 255 / leds * j;
		CRGB color = ColorFromPalette(palette[paletteNo], colorindex);
		int pixelnumber;
		for (int i = 0; i < 4; ++i) {
			pixelnumber = (leds - 1) - j;
			int pixelIdx = ((pixelnumber) * LEDS_WIDTH + i) * 3;

			if (j < amp) {
				imageData[pixelIdx + 0] = color.r;
				imageData[pixelIdx + 1] = color.g;
				imageData[pixelIdx + 2] = color.b;
			} else {
				imageData[pixelIdx + 0] = imageData[pixelIdx + 1] =
						imageData[pixelIdx + 2] = 0;
			}
		}
	}

	*pAmplitude *= 0.85f;
//	*pAmplitude = 0.0f;
}

void VUMeterVisualizer::start() {
}

void VUMeterVisualizer::stop() {
}

boolean VUMeterVisualizer::onButtonPressed(uint8_t button) {
	boolean handled = false;

	switch (button) {
	case 28:
		handled = true;
		if (rendererId > 0)
			rendererId--;
		delay(200);
		break;
	case 32:
		handled = true;
		if (rendererId < 2)
			rendererId++;
		delay(200);
		break;

	case 25:
		cooling -= 5;
		if (cooling <= 0)
			cooling = 0;
		handled = true;
		break;
	case 29:
		cooling += 5;
		if (cooling >= 1000)
			cooling = 1000;
		handled = true;
		break;
	case 26:
		sparking += 5;
		if (sparking >= 250)
			sparking = 250;
		handled = true;
		break;
	case 30:
		sparking -= 5;
		if (sparking <= 0)
			sparking = 0;
		handled = true;
		break;

	case 27:
		paletteMax += 5;
		if (paletteMax > 240)
			paletteMax = 240;
		handled = true;
		break;
	case 31:
		paletteMax -= 5;
		if (paletteMax < 50)
			paletteMax = 50;
		handled = true;
		break;

	case 33:
		paletteNo = 0;
		handled = true;
		break;
	case 34:
		paletteNo = 1;
		handled = true;
		break;
	case 35:
		paletteNo = 2;
		handled = true;
		break;
	case 36:
		paletteNo = 3;
		handled = true;
		break;
	case 37:
		paletteNo = 4;
		handled = true;
		break;
	case 38:
		paletteNo = 5;
		handled = true;
		break;
	case 39:
		paletteNo = 6;
		handled = true;
		break;
	case 40:
		paletteNo = 7;
		handled = true;
		break;
	}

	return handled;
}
