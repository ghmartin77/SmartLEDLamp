#include "VisualizerApp.h"
#include "defines.h"

VisualizerApp::VisualizerApp(LEDMatrix* pLEDMatrix) :
		App(pLEDMatrix), lastMillis(0) {
	name = "Visualizer";
}

VisualizerApp::~VisualizerApp() {
}

void VisualizerApp::setVisualizer(uint8_t idx, Visualizer* pVisualizer) {
	visualizers[idx] = pVisualizer;
}

void VisualizerApp::start() {
	for (int i = 0; i < 2; ++i) {
		if (visualizers[i]) {
			visualizers[i]->start();
		}
	}
}

void VisualizerApp::stop() {
	for (int i = 0; i < 2; ++i) {
		if (visualizers[i]) {
			visualizers[i]->stop();
		}
	}
}

boolean VisualizerApp::onButtonPressed(uint8_t button) {
	boolean handled = false;

	for (int i = 0; i < 2; ++i) {
		if (visualizers[i]) {
			handled |= visualizers[i]->onButtonPressed(button);
		}
	}

	return handled;
}

void VisualizerApp::update() {
	for (int i = 0; i < LEDS_WIDTH * LEDS_HEIGHT; ++i) {
		uint16_t r = 0, g = 0, b = 0;
		for (int j = 0; j < 2; ++j) {
			if (pImageData[j]) {
				r += pImageData[j][i * 3];
				if (r > 255)
					r = 255;

				g += pImageData[j][i * 3 + 1];
				if (g > 255)
					g = 255;

				b += pImageData[j][i * 3 + 2];
				if (b > 255)
					b = 255;
			}
		}
		pMatrix->setPixel(i % LEDS_WIDTH, i / LEDS_WIDTH, (uint8_t) (r),
				(uint8_t) (g), (uint8_t) (b));
	}
	pMatrix->update();
}

void VisualizerApp::run() {
	unsigned long int currentMillis = millis();

	// run with 25 fps
	if (currentMillis - 40 < lastMillis) {
		return;
	}

	lastMillis = currentMillis;

	for (int i = 0; i < 2; ++i) {
		if (visualizers[i]) {
			pImageData[i] = visualizers[i]->renderNextImage();
		} else {
			pImageData[i] = NULL;
		}
	}

	update();
}
