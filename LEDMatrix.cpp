#include "LEDMatrix.h"

LEDMatrix::LEDMatrix(uint8_t dpin, uint8_t cpin, uint8_t aWidth,
		uint8_t aHeight) :
		width(aWidth), height(aHeight), noPixels(aWidth * aHeight), matrix(
				Adafruit_WS2801(aHeight, aWidth, dpin, cpin,
				MATRIX_RGB_FORMAT)), pixels(
				(uint8_t*) malloc(3 * aWidth * aHeight)) {

}

LEDMatrix::~LEDMatrix() {
	if (pixels)
		free(pixels);
}

void LEDMatrix::init() {
	matrix.begin();
}

void LEDMatrix::clear() {
	fill(0, 0, 0);
}

void LEDMatrix::fill(uint8_t r, uint8_t g, uint8_t b) {
	for (uint16_t i = 0; i < noPixels * 3; i += 3) {
		pixels[i] = r;
		pixels[i + 1] = g;
		pixels[i + 2] = b;
	}
	update();
}

void LEDMatrix::setPixel(int16_t x, int16_t y, uint8_t r, uint8_t g,
		uint8_t b) {
	if (x >= width || y >= height || x < 0 || y < 0)
		return;

	uint16_t idx = (x + y * width) * 3;
	pixels[idx] = r;
	pixels[idx + 1] = g;
	pixels[idx + 2] = b;
}

void LEDMatrix::setRotation(Rotation rot) {
	rotation = rot;
}

void LEDMatrix::setCalibration(float r, float g, float b) {
	calibrationRed = r;
	calibrationGreen = g;
	calibrationBlue = b;
}

void LEDMatrix::setBrightness(float percentage) {
	brightness = percentage;
}

void LEDMatrix::update() {
	uint16_t x = 0;
	uint16_t y = 0;

	float calibrationRedEffectively = calibrationRed * brightness;
	float calibrationGreenEffectively = calibrationGreen * brightness;
	float calibrationBlueEffectively = calibrationBlue * brightness;

	if (rotation == ROTATION_180) {
		for (uint16_t idx = 0; idx < noPixels * 3; idx = idx + 3) {
			matrix.setPixelColor(y, x, pixels[idx] * calibrationRedEffectively,
					pixels[idx + 1] * calibrationGreenEffectively,
					pixels[idx + 2] * calibrationBlueEffectively);
			if (++x >= width) {
				++y;
				x = 0;
			}
		}
	} else { // ROTATION_0
		for (uint16_t idx = 0; idx < noPixels * 3; idx = idx + 3) {

			int16_t currentY = height - 1 - y - yOffset;

			if (currentY >= 0) {
				matrix.setPixelColor(currentY, x,
						pixels[idx] * calibrationRedEffectively,
						pixels[idx + 1] * calibrationGreenEffectively,
						pixels[idx + 2] * calibrationBlueEffectively);
			} else {
				matrix.setPixelColor(currentY + height, x, 0, 0, 0);
			}
			if (++x >= width) {
				++y;
				x = 0;
			}
		}
	}
	matrix.show();
}

void LEDMatrix::flip() {
	switch (rotation) {
	case ROTATION_0:
		setRotation(ROTATION_180);
		break;
	case ROTATION_180:
	default:
		setRotation(ROTATION_0);
		break;
	}

	update();
}
