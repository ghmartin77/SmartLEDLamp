#include "DemoReelVisualizer.h"

DemoReelVisualizer::DemoReelVisualizer() {
}

DemoReelVisualizer::~DemoReelVisualizer() {
}

void DemoReelVisualizer::computeImage() {
	EVERY_N_MILLISECONDS(20) {
		hue++;
	}

	switch (currentPatternNumber) {
	case 0:
		confetti();
		break;
	case 1:
		sinelon();
		break;
	case 2:
		bpm();
		break;
	default:
		juggle();
		break;
	}

	for (int i = 0; i < LEDS_HEIGHT; ++i)
		for (int x = 0; x < LEDS_WIDTH; ++x) {
			imageData[((i) * LEDS_WIDTH + x) * 3 + 0] = leds[i].r;
			imageData[((i) * LEDS_WIDTH + x) * 3 + 1] = leds[i].g;
			imageData[((i) * LEDS_WIDTH + x) * 3 + 2] = leds[i].b;
		}
}

void DemoReelVisualizer::confetti() {
	fadeToBlackBy(leds, LEDS_HEIGHT, 10);
	int pos = random16(LEDS_HEIGHT);
	leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void DemoReelVisualizer::rainbow() {
	fill_rainbow(leds, LEDS_HEIGHT, hue, 7);
}

void DemoReelVisualizer::sinelon() {
	fadeToBlackBy(leds, LEDS_HEIGHT, 20);
	int pos = beatsin16(16, 0, LEDS_HEIGHT);
	CHSV chsv = CHSV(hue, 255, 192);

	if (lastPos != -1) {
		for (int i = min(lastPos, pos); i < max(lastPos,pos); ++i)
		{
			leds[i] += chsv;
		}
	}

	lastPos = pos;
}

void DemoReelVisualizer::bpm() {
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
	for (int i = 0; i < LEDS_HEIGHT; i++) {
		leds[i] = ColorFromPalette(palette, hue + (i * 2),
				beat - hue + (i * 10));
	}
}

void DemoReelVisualizer::juggle() {
	fadeToBlackBy(leds, LEDS_HEIGHT, 20);
	byte dothue = 0;
	for (int i = 0; i < 3; i++) {
		leds[beatsin16(i + 8, 0, LEDS_HEIGHT)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void DemoReelVisualizer::start() {
	randomSeed(micros());
}

void DemoReelVisualizer::stop() {
}

boolean DemoReelVisualizer::onButtonPressed(uint8_t button) {
	boolean handled = false;

	switch (button) {
	case BTN_RED_UP:
		handled = true;
		currentPatternNumber += 1;
		if (currentPatternNumber > 5)
			currentPatternNumber = 5;
		else
			delay(200);
		break;
	case BTN_RED_DOWN:
		handled = true;
		currentPatternNumber -= 1;
		if (currentPatternNumber < 0)
			currentPatternNumber = 0;
		else
			delay(200);
		break;
	}

	return handled;
}
