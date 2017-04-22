#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#include "Adafruit_WS2801.h"

#define MATRIX_RGB_FORMAT WS2801_RBG

#include <Adafruit_WS2801.h>

class LEDMatrix {
public:
	enum Rotation {
		ROTATION_0, ROTATION_180
	};

	LEDMatrix(uint8_t dpin, uint8_t cpin, uint8_t aWidth, uint8_t aHeight);
	virtual ~LEDMatrix();

	void init();

	void setPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);

	void clear();
	void fill(uint8_t r, uint8_t g, uint8_t b);
	void update();

	void flip();

	void setRotation(Rotation rot);
	void setCalibration(float r, float g, float b);
	void setBrightness(float percentage);

	inline uint8_t getWidth() {
		return width;
	}
	inline uint8_t getHeight() {
		return height;
	}

	inline void setYOffset(uint8_t offset) {
		if (offset > height) {
			offset = height;
		}
		yOffset = offset;
	}

	inline uint8_t getYOffset() {
		return yOffset;
	}

private:
	Adafruit_WS2801 matrix;

	uint8_t *pixels;
	uint8_t width;
	uint8_t height;
	uint8_t yOffset = 0;
	uint8_t noPixels;

	Rotation rotation = ROTATION_0;

	float calibrationRed = 1.0f;
	float calibrationGreen = 1.0f;
	float calibrationBlue = 1.0f;
	float brightness = 1.0f;
};

#endif /* LEDMATRIX_H_ */
