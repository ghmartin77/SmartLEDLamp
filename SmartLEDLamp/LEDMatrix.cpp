#include "LEDMatrix.h"
#include "defines.h"

LEDMatrix::LEDMatrix(uint8_t aWidth, uint8_t aHeight) :
		width(aWidth), height(aHeight), noPixels(aWidth * aHeight), pLEDs(
				(CRGB*) malloc(aWidth * aHeight * sizeof(CRGB))), pPixels(
				(CRGB*) malloc(aWidth * aHeight * sizeof(CRGB))) {

}

LEDMatrix::~LEDMatrix() {
	if (pPixels)
		free(pPixels);
	if (pLEDs)
		free(pLEDs);
}

void LEDMatrix::init() {
	FastLED.addLeds<STRIP_CHIPSET, PIN_DATA, PIN_CLOCK, STRIP_RGB_ORDER>(pLEDs,
			width * height);
}

void LEDMatrix::clear() {
	fill(0, 0, 0);
}

void LEDMatrix::fill(uint8_t r, uint8_t g, uint8_t b) {
	for (uint16_t i = 0; i < noPixels; ++i) {
		pPixels[i].r = r;
		pPixels[i].g = g;
		pPixels[i].b = b;
	}
	update();
}

void LEDMatrix::setPixel(int16_t x, int16_t y, uint8_t r, uint8_t g,
		uint8_t b) {
	if (x >= width || y >= height || x < 0 || y < 0)
		return;

	uint16_t idx = x + y * width;
	pPixels[idx].r = r;
	pPixels[idx].g = g;
	pPixels[idx].b = b;
}

void LEDMatrix::setCalibration(float r, float g, float b) {
	calibrationRed = r;
	calibrationGreen = g;
	calibrationBlue = b;

	const CRGB colorCalibration(255 * r, 255 * g, 255 * b);
	FastLED.setCorrection(colorCalibration);
}

void LEDMatrix::setBrightness(float percentage) {
	brightness = percentage;

	FastLED.setBrightness(255 * percentage);
}

void LEDMatrix::update() {
	uint16_t idxSrc = 0;

	uint8_t curOffset = yOffset;

	for (uint8_t y = 0; y < height; ++y) {
		for (uint8_t x = 0; x < width; ++x) {
			CRGB& led = pLEDs[xy2idx(x, y)];

			int16_t readY = y - yOffset;
			if (readY < 0) {
				led = 0;
			} else {
				led = pPixels[x + readY * width];
			}
		}
	}

	FastLED.show();
}

