#include "FireVisualizer.h"

FireVisualizer::FireVisualizer() :
		cooling(95), sparking(200), paletteNo(0), paletteMax(240) {
}

FireVisualizer::~FireVisualizer() {
}

void FireVisualizer::computeImage() {
	random16_add_entropy(random(-1));

	switch (rendererId) {
	case 0:
		computeImage3();
		break;
	case 1:
		computeImage1();
		break;
	case 2:
		computeImage2();
		break;
	}
}

void FireVisualizer::computeImage1() {
	int8_t leds = LEDS_HEIGHT * 2;

	for (int x = 0; x < LEDS_WIDTH; x++) {

		// Step 1.  Cool down every cell a little
		for (int i = 0; i < leds; i++) {
			heat[x][i] = qsub8(heat[x][i],
					random8(0, ((cooling * 10.0f) / leds) + 2));
		}

		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (int k = leds - 1; k >= 2; k--) {
			heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
		}

		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		if (random8() < sparking) {
			int y = random8(7);
			heat[x][y] = qadd8(heat[x][y], random8(160, 255));
		}

		// Step 4.  Map from heat cells to LED colors
		for (int j = 0; j < leds; j++) {
			// Scale the heat value from 0-255 down to 0-240
			// for best results with color palettes.
			byte colorindex = scale8(heat[x][j], paletteMax);
			CRGB color = ColorFromPalette(palette[paletteNo], colorindex);
			int pixelnumber;
			pixelnumber = (leds - 1) - j;
			int pixelIdx = ((pixelnumber) * LEDS_WIDTH + x) * 3;
			vImageData[pixelIdx + 0] = color.r;
			vImageData[pixelIdx + 1] = color.g;
			vImageData[pixelIdx + 2] = color.b;
		}

		for (int j = 0; j < LEDS_HEIGHT; j++) {
			int16_t rgbIndex = (j * LEDS_WIDTH + x) * 3;
			int16_t r, g, b;
			r = vImageData[rgbIndex * 2 + 0];
			g = vImageData[rgbIndex * 2 + 1];
			b = vImageData[rgbIndex * 2 + 2];

			if (j > 0) {
				r += vImageData[rgbIndex * 2 + 0 - 12];
				g += vImageData[rgbIndex * 2 + 1 - 12];
				b += vImageData[rgbIndex * 2 + 2 - 12];
			}

			r += vImageData[rgbIndex * 2 + 0 + 12];
			g += vImageData[rgbIndex * 2 + 1 + 12];
			b += vImageData[rgbIndex * 2 + 2 + 12];

			if (j > 0) {
				r = r / 3;
				g = g / 3;
				b = b / 3;
			} else {
				r = r / 2;
				g = g / 2;
				b = b / 2;
			}

			if (r > 255)
				r = 255;
			if (g > 255)
				g = 255;
			if (b > 255)
				b = 255;

			imageData[rgbIndex + 0] = r;
			imageData[rgbIndex + 1] = g;
			imageData[rgbIndex + 2] = b;
		}
	}
}

void FireVisualizer::computeImage2() {
	int8_t leds = LEDS_HEIGHT * 2;

	for (int x = 0; x < LEDS_WIDTH; x++) {

		// Step 1.  Cool down every cell a little
		for (int i = 0; i < leds; i++) {
			heat[x][i] = qsub8(heat[x][i],
					random8(0, ((cooling * 10) / leds) + 2));
		}

		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (int k = leds - 1; k >= 2; k--) {
			heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
		}

		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		if (random8() < sparking) {
			int y = random8(7);
			heat[x][y] = qadd8(heat[x][y], random8(160, 255));
		}

		// Step 4.  Map from heat cells to LED colors
		for (int j = 0; j < leds; j++) {
			// Scale the heat value from 0-255 down to 0-240
			// for best results with color palettes.
			byte colorindex = scale8(heat[x][j], paletteMax);
			CRGB color = ColorFromPalette(palette[paletteNo], colorindex);
			int pixelnumber;
			pixelnumber = (leds - 1) - j;
			int pixelIdx = ((pixelnumber) * LEDS_WIDTH + x) * 3;
			vImageData[pixelIdx + 0] = color.r;
			vImageData[pixelIdx + 1] = color.g;
			vImageData[pixelIdx + 2] = color.b;
		}

		for (int j = 0; j < LEDS_HEIGHT; j++) {
			int16_t rgbIndex = (j * LEDS_WIDTH + x) * 3;
			int16_t r, g, b;
			r = vImageData[rgbIndex * 2 + 0];
			g = vImageData[rgbIndex * 2 + 1];
			b = vImageData[rgbIndex * 2 + 2];

			if (j > 0) {
				r += vImageData[rgbIndex * 2 + 0 - 12] / 2;
				g += vImageData[rgbIndex * 2 + 1 - 12] / 2;
				b += vImageData[rgbIndex * 2 + 2 - 12] / 2;
			}

			r += vImageData[rgbIndex * 2 + 0 + 12] / 2;
			g += vImageData[rgbIndex * 2 + 1 + 12] / 2;
			b += vImageData[rgbIndex * 2 + 2 + 12] / 2;

			if (r > 255)
				r = 255;
			if (g > 255)
				g = 255;
			if (b > 255)
				b = 255;

			imageData[rgbIndex + 0] = r;
			imageData[rgbIndex + 1] = g;
			imageData[rgbIndex + 2] = b;
		}
	}
}

void FireVisualizer::computeImage3() {
	int8_t leds = LEDS_HEIGHT;

	for (int x = 0; x < LEDS_WIDTH; x++) {

		// Step 1.  Cool down every cell a little
		for (int i = 0; i < leds; i++) {
			heat[x][i] = qsub8(heat[x][i],
					random8(0, ((cooling * 10) / leds) + 2));
		}

		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (int k = leds - 1; k >= 2; k--) {
			heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
		}

		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		if (random8() < sparking) {
			int y = random8(7);
			heat[x][y] = qadd8(heat[x][y], random8(160, 255));
		}

		// Step 4.  Map from heat cells to LED colors
		for (int j = 0; j < leds; j++) {
			// Scale the heat value from 0-255 down to 0-240
			// for best results with color palettes.
			byte colorindex = scale8(heat[x][j], paletteMax);
			CRGB color = ColorFromPalette(palette[paletteNo], colorindex);
			int pixelnumber;
			pixelnumber = (leds - 1) - j;
			int pixelIdx = ((pixelnumber) * LEDS_WIDTH + x) * 3;
			imageData[pixelIdx + 0] = color.r;
			imageData[pixelIdx + 1] = color.g;
			imageData[pixelIdx + 2] = color.b;
		}
	}
}

void FireVisualizer::start() {
	randomSeed(micros());
}

void FireVisualizer::stop() {
}

boolean FireVisualizer::onButtonPressed(uint8_t button) {
	boolean handled = false;

	switch (button) {
	case BTN_QUICK:
		handled = true;
		if (rendererId > 0)
			rendererId--;
		delay(200);
		break;
	case BTN_SLOW:
		handled = true;
		if (rendererId < 2)
			rendererId++;
		delay(200);
		break;

	case BTN_RED_UP:
		cooling -= 5;
		if (cooling <= 0)
			cooling = 0;
		handled = true;
		break;
	case BTN_RED_DOWN:
		cooling += 5;
		if (cooling >= 1000)
			cooling = 1000;
		handled = true;
		break;

	case BTN_GREEN_UP:
		sparking += 5;
		if (sparking >= 250)
			sparking = 250;
		handled = true;
		break;
	case BTN_GREEN_DOWN:
		sparking -= 5;
		if (sparking <= 0)
			sparking = 0;
		handled = true;
		break;

	case BTN_BLUE_UP:
		paletteMax += 5;
		if (paletteMax > 240)
			paletteMax = 240;
		handled = true;
		break;
	case BTN_BLUE_DOWN:
		paletteMax -= 5;
		if (paletteMax < 50)
			paletteMax = 50;
		handled = true;
		break;

	case BTN_DIY1:
		paletteNo = 0;
		handled = true;
		break;
	case BTN_DIY2:
		paletteNo = 1;
		handled = true;
		break;
	case BTN_DIY3:
		paletteNo = 2;
		handled = true;
		break;
	case BTN_AUTO:
		paletteNo = 3;
		handled = true;
		break;
	case BTN_DIY4:
		paletteNo = 4;
		handled = true;
		break;
	case BTN_DIY5:
		paletteNo = 5;
		handled = true;
		break;
	case BTN_DIY6:
		paletteNo = 6;
		handled = true;
		break;
	}

	return handled;
}

const TProgmemRGBPalette16 HeatColorsBlue_p FL_PROGMEM = { 0x000000, 0x000033,
		0x000066, 0x000099, 0x0000cc, 0x0000ff, 0x0033FF, 0x0066FF, 0x0099FF,
		0x00CCFF, 0x00FFFF, 0x33FFFF, 0x66FFFF, 0x99FFFF, 0xccFFFF, 0xFFFFFF };

const TProgmemRGBPalette16 HeatColorsGreen_p FL_PROGMEM = { 0x000000, 0x003300,
		0x006600, 0x009900, 0x00CC00, 0x00FF00, 0x00FF33, 0x00FF66, 0x00FF99,
		0x00FFCC, 0x00FFFF, 0x33FFFF, 0x66FFFF, 0x99FFFF, 0xccFFFF, 0xFFFFFF };
