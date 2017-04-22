/*
 * TurnOffRunnable.h
 */

#ifndef TURNOFFRUNNABLE_H_
#define TURNOFFRUNNABLE_H_

#include "Runnable.h"
#include "LEDMatrix.h"

class TurnOffRunnable: public Runnable {
public:
	TurnOffRunnable(LEDMatrix* pLEDMatrix);
	virtual ~TurnOffRunnable();

	void init();
	virtual void run();

protected:
	uint16_t currentOffset = 0;
};

#endif /* TURNOFFRUNNABLE_H_ */
