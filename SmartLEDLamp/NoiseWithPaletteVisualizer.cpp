#include "NoiseWithPaletteVisualizer.h"

NoiseWithPaletteVisualizer::NoiseWithPaletteVisualizer() :
		currentPalette(PartyColors_p) {
}

NoiseWithPaletteVisualizer::~NoiseWithPaletteVisualizer() {
}

void NoiseWithPaletteVisualizer::computeImage() {
	// generate noise data
	fillnoise8();

	// convert the noise data to colors in the LED array
	// using the current palette
	mapNoiseToLEDsUsingPalette();
}

void NoiseWithPaletteVisualizer::fillnoise8() {
	// If we're running at a low "speed", some 8-bit artifacts become visible
	// from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
	// The amount of data smoothing we're doing depends on "speed".
	uint8_t dataSmoothing = 0;
	if (speed < 50) {
		dataSmoothing = 200 - (speed * 4);
	}

	for (int i = 0; i < LEDS_HEIGHT; i++) {
		int ioffset = scale * i;
		for (int j = 0; j < LEDS_HEIGHT; j++) {
			int joffset = scale * j;

			uint8_t data = inoise8(x + ioffset, y + joffset, z);

			// The range of the inoise8 function is roughly 16-238.
			// These two operations expand those values out to roughly 0..255
			// You can comment them out if you want the raw noise data.
			data = qsub8(data, 16);
			data = qadd8(data, scale8(data, 39));

			if (dataSmoothing) {
				uint8_t olddata = noise[i][j];
				uint8_t newdata = scale8(olddata, dataSmoothing)
						+ scale8(data, 256 - dataSmoothing);
				data = newdata;
			}

			noise[i][j] = data;
		}
	}

	z += speed;

	// apply slow drift to X and Y, just for visual variation.
	x += speed / 8;
	y -= speed / 16;
}

void NoiseWithPaletteVisualizer::mapNoiseToLEDsUsingPalette() {
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 4; j++) {
			// We use the value at the (i,j) coordinate in the noise
			// array for our brightness, and the flipped value from (j,i)
			// for our pixel's index into the color palette.

			uint8_t index = noise[j][i];
			uint8_t bri = noise[i][j];

			// if this palette is a 'loop', add a slowly-changing base value
			if (colorLoop) {
				index += ihue;
			}

			// brighten up, as the color palette itself often contains the
			// light/dark dynamic range desired
			if (bri > 127) {
				bri = 255;
			} else {
				bri = dim8_raw(bri * 2);
			}

			CRGB crgb = ColorFromPalette(currentPalette, index, bri);
			imageData[((i) * LEDS_WIDTH + j) * 3 + 0] = crgb.r;
			imageData[((i) * LEDS_WIDTH + j) * 3 + 1] = crgb.g;
			imageData[((i) * LEDS_WIDTH + j) * 3 + 2] = crgb.b;
		}
	}

	ihue += 1;
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.
// 1 = 5 sec per palette
// 2 = 10 sec per palette
// etc
#define HOLD_PALETTES_X_TIMES_AS_LONG 1

void NoiseWithPaletteVisualizer::applyProfile() {
	if (profile == 1) {
		currentPalette = RainbowColors_p;
		colorLoop = 1;
	} else if (profile == 2) {
		SetupPurpleAndGreenPalette();
		colorLoop = 1;
	} else if (profile == 3) {
		SetupBlackAndWhiteStripedPalette();
		colorLoop = 1;
	} else if (profile == 4) {
		currentPalette = ForestColors_p;
		colorLoop = 0;
	} else if (profile == 5) {
		currentPalette = CloudColors_p;
		colorLoop = 0;
	} else if (profile == 6) {
		currentPalette = LavaColors_p;
		colorLoop = 0;
	} else if (profile == 7) {
		currentPalette = OceanColors_p;
		colorLoop = 0;
	} else if (profile == 8) {
		currentPalette = PartyColors_p;
		colorLoop = 1;
	} else if (profile == 9) {
		currentPalette = RainbowStripeColors_p;
		colorLoop = 1;
	}
}

// This function generates a random palette that's a gradient between four different colors.  The first is a dim hue, the second is
// a bright hue, the third is a bright pastel, and the last is
// another bright hue.  This gives some visual bright/dark variation
// which is more interesting than just a gradient of different hues.
void NoiseWithPaletteVisualizer::SetupRandomPalette() {
	currentPalette = CRGBPalette16(CHSV(random8(), 255, 32),
			CHSV(random8(), 255, 255), CHSV(random8(), 128, 255),
			CHSV(random8(), 255, 255));
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void NoiseWithPaletteVisualizer::SetupBlackAndWhiteStripedPalette() {
// 'black out' all 16 palette entries...
	fill_solid(currentPalette, 16, CRGB::Black);
// and set every fourth one to white.
	currentPalette[0] = CRGB::White;
	currentPalette[4] = CRGB::White;
	currentPalette[8] = CRGB::White;
	currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void NoiseWithPaletteVisualizer::SetupPurpleAndGreenPalette() {
	CRGB purple = CHSV(HUE_PURPLE, 255, 255);
	CRGB green = CHSV(HUE_GREEN, 255, 255);
	CRGB black = CRGB::Black;

	currentPalette = CRGBPalette16(green, green, black, black, purple, purple,
			black, black, green, green, black, black, purple, purple, black,
			black);
}

void NoiseWithPaletteVisualizer::start() {
	x = random16();
	y = random16();
	z = random16();
}

void NoiseWithPaletteVisualizer::stop() {
}

boolean NoiseWithPaletteVisualizer::onButtonPressed(uint8_t button) {
	boolean handled = false;

	switch (button) {
	case BTN_QUICK:
		handled = true;
		if (speed < 60)
			speed++;
		break;
	case BTN_SLOW:
		handled = true;
		if (speed > 0)
			speed--;
		break;

	case BTN_BLUE_DOWN:
		handled = true;
		scale -= 3;
		if (scale <= 4)
			scale = 4;
		break;
	case BTN_BLUE_UP:
		handled = true;
		scale += 3;
		if (scale >= 100)
			scale = 100;
		break;

	case BTN_RED_DOWN:
		handled = true;
		if (profile > 1) {
			profile--;
			applyProfile();
			delay(200);
		}
		break;
	case BTN_RED_UP:
		handled = true;
		if (profile < 9) {
			profile++;
			applyProfile();
			delay(200);
		}
		break;
	}

	return handled;
}
