/*
 * TurnOffRunnable.cpp
 */

#include "defines.h"
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
	unsigned long currentMillis = millis();

	if (currentMillis - lastMillis < 10) {
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

#ifdef MQTT_ENABLE
		mqttClient.publish(MQTT_TOPIC, "0", true);
#endif // MQTT_ENABLE
	}
}

