/*
 * TurnOnRunnable.cpp
 */

#include "TurnOnRunnable.h"
#include "LEDLamp.h"
#include <stddef.h>

TurnOnRunnable::TurnOnRunnable(LEDMatrix* pLEDMatrix) :
		Runnable(pLEDMatrix) {
}

TurnOnRunnable::~TurnOnRunnable() {
}

void TurnOnRunnable::init() {
	currentOffset = pMatrix->getYOffset();
	if (currentOffset == 0) {
		currentOffset = pMatrix->getHeight();
	}
	pMatrix->setYOffset(currentOffset);
}

void TurnOnRunnable::run() {
	if (!isOn) {
		isOn = true;
#ifdef MQTT_ENABLE
		mqttClient.publish(MQTT_TOPIC, "1", true);
#endif // MQTT_ENABLE
	}

	unsigned long int currentMillis = millis();

	if (currentMillis - 10 < lastMillis) {
		return;
	}

	lastMillis = currentMillis;

	if (currentOffset > 0) {
		currentOffset = currentOffset - 2;
		pMatrix->setYOffset(currentOffset);
	} else {
		pMatrix->setYOffset(0);
		pCurrentRunnable = NULL;
	}
}

