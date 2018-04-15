/*
 * TurnOffRunnable.cpp
 */

#include "TurnOffRunnable.h"
#include "LEDLamp.h"
#include <stddef.h>

TurnOffRunnable::TurnOffRunnable(LEDMatrix* pLEDMatrix) :
		Runnable(pLEDMatrix) {
}

TurnOffRunnable::~TurnOffRunnable() {
}

void TurnOffRunnable::init() {
	currentOffset = pMatrix->getYOffset();
}

void TurnOffRunnable::run() {
	unsigned long int currentMillis = millis();

	if (currentMillis - 10 < lastMillis) {
		return;
	}

	lastMillis = currentMillis;

	if (currentOffset < pMatrix->getHeight()) {
		currentOffset = currentOffset + 2;
		pMatrix->setYOffset(currentOffset);
	} else {
		pMatrix->setYOffset(pMatrix->getHeight());
		pCurrentRunnable = NULL;
		isOn = false;
	}
}

