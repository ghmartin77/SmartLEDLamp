#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#define FASTLED_INTERNAL
#include "FastLED.h"

class LEDMatrix {
public:
	LEDMatrix(uint8_t aWidth, uint8_t aHeight);
	virtual ~LEDMatrix();

	void init();

	void setPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);

	void clear();
	void fill(uint8_t r, uint8_t g, uint8_t b);
	void update();

	void flip();

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

	inline uint16_t xy2idx(uint8_t x, uint8_t y) {
		uint16_t idx;

		if (serpentineLayout && !(x & 1)) {
			idx = (x * height) + (height - 1) - y;
		} else {
			idx = (x * height) + y;
		}

		return idx;
	}

private:
	CRGB* pLEDs;

	CRGB* pPixels;
	uint8_t width;
	uint8_t height;
	uint8_t yOffset = 0;
	uint8_t noPixels;

	float calibrationRed = 1.0f;
	float calibrationGreen = 1.0f;
	float calibrationBlue = 1.0f;
	float brightness = 1.0f;

	bool serpentineLayout = true;
};

#endif /* LEDMATRIX_H_ */
