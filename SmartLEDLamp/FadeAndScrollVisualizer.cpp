#include "FadeAndScrollVisualizer.h"
#include "defines.h"
#include <EEPROM.h>
#include "Log.h"

FadeAndScrollVisualizer::FadeAndScrollVisualizer(uint8_t _fsSpeed,
		uint8_t _fsZoom) :
		fsSpeed(_fsSpeed), fsZoom(_fsZoom), pFsHeight(NULL), pFsLut(NULL), fsIndex(
				0), fsHeightOffset(0) {
}

FadeAndScrollVisualizer::~FadeAndScrollVisualizer() {
}

void FadeAndScrollVisualizer::computeImage() {
	for (int x = 0; x < LEDS_WIDTH; x++) {
		for (int y = 0; y < LEDS_HEIGHT; y++) {
			fsIndex = y * LEDS_WIDTH + x;
			pFsHeight[fsIndex] += fsSpeed;
			if (pFsHeight[fsIndex] >= 1536) {
				pFsHeight[fsIndex] = pFsHeight[fsIndex] - 1536;
			}
			imageData[fsIndex * 3 + 0] = pFsLut[pFsHeight[fsIndex] * 3 + 0];
			imageData[fsIndex * 3 + 1] = pFsLut[pFsHeight[fsIndex] * 3 + 1];
			imageData[fsIndex * 3 + 2] = pFsLut[pFsHeight[fsIndex] * 3 + 2];
		}
	}
}

void FadeAndScrollVisualizer::updateFsHeight() {
	for (int x = 0; x < LEDS_WIDTH; x++) {
		for (int y = 0; y < LEDS_HEIGHT; y++) {
			fsIndex = y * LEDS_WIDTH + x;
			pFsHeight[fsIndex] = (fsHeightOffset + (y * fsZoom)) % 1536;
		}
	}
}

void FadeAndScrollVisualizer::start() {
	fsIndex = 0;

	pFsLut = (uint8_t*) malloc(sizeof(uint8_t) * 3 * 1536);
	pFsHeight = (int16_t*) malloc(sizeof(int16_t) * LEDS_WIDTH * LEDS_HEIGHT);

	for (int i = 0; i < 256; ++i) {
		pFsLut[(i + 0) * 3 + 0] = 255;
		pFsLut[(i + 0) * 3 + 1] = i;
		pFsLut[(i + 0) * 3 + 2] = 0;

		pFsLut[(i + 256) * 3 + 0] = 255 - i;
		pFsLut[(i + 256) * 3 + 1] = 255;
		pFsLut[(i + 256) * 3 + 2] = 0;

		pFsLut[(i + 512) * 3 + 0] = 0;
		pFsLut[(i + 512) * 3 + 1] = 255;
		pFsLut[(i + 512) * 3 + 2] = i;

		pFsLut[(i + 768) * 3 + 0] = 0;
		pFsLut[(i + 768) * 3 + 1] = 255 - i;
		pFsLut[(i + 768) * 3 + 2] = 255;

		pFsLut[(i + 1024) * 3 + 0] = i;
		pFsLut[(i + 1024) * 3 + 1] = 0;
		pFsLut[(i + 1024) * 3 + 2] = 255;

		pFsLut[(i + 1280) * 3 + 0] = 255;
		pFsLut[(i + 1280) * 3 + 1] = 0;
		pFsLut[(i + 1280) * 3 + 2] = 255 - i;
	}

	updateFsHeight();
	computeImage();
}

void FadeAndScrollVisualizer::stop() {
	if (pFsLut)
		free(pFsLut);
	if (pFsHeight)
		free(pFsHeight);
}

boolean FadeAndScrollVisualizer::onButtonPressed(uint8_t button) {
	boolean handled = false;

	switch (button) {
	case BTN_QUICK:
		handled = true;
		fsSpeed += 1;
		if (fsSpeed >= 40)
			fsSpeed = 40;
		break;
	case BTN_SLOW:
		handled = true;
		fsSpeed -= 1;
		if (fsSpeed <= 1)
			fsSpeed = 1;
		break;

	case BTN_BLUE_UP:
		handled = true;
		fsZoom -= 5;
		if (fsZoom <= 1)
			fsZoom = 1;
		else
			updateFsHeight();
		break;
	case BTN_BLUE_DOWN:
		handled = true;
		fsZoom += 5;
		if (fsZoom >= 250)
			fsZoom = 250;
		else
			updateFsHeight();
		break;
	}

	return handled;
}

void FadeAndScrollVisualizer::readRuntimeConfiguration(int &address) {
	Logger.debug("FadeAndScrollVisualizer::readRuntimeConfiguration");
	Visualizer::readRuntimeConfiguration(address);
	EEPROM.get(address, fsSpeed);
	address += sizeof(fsSpeed);
	EEPROM.get(address, fsZoom);
	address += sizeof(fsZoom);
	EEPROM.get(address, fsHeightOffset);
	address += sizeof(fsHeightOffset);

	// adjust offset to cope for modification in computeImage()
	fsHeightOffset -= fsSpeed;
	if (fsHeightOffset < 0) {
		fsHeightOffset += 1536;
	}

}

void FadeAndScrollVisualizer::writeRuntimeConfiguration(int &address) {
	Logger.debug("FadeAndScrollVisualizer::writeRuntimeConfiguration");

	fsHeightOffset = 0;

	if (pFsHeight) {
		fsHeightOffset = pFsHeight[0];
	}

	Visualizer::writeRuntimeConfiguration(address);
	EEPROM.put(address, fsSpeed);
	address += sizeof(fsSpeed);
	EEPROM.put(address, fsZoom);
	address += sizeof(fsZoom);
	EEPROM.put(address, fsHeightOffset);
	address += sizeof(fsHeightOffset);
}

