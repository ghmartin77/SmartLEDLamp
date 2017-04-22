/*
 * TurnOnRunnable.h
 */

#ifndef TURNONRUNNABLE_H_
#define TURNONRUNNABLE_H_

#include "Runnable.h"
#include "LEDMatrix.h"

class TurnOnRunnable: public Runnable {
public:
	TurnOnRunnable(LEDMatrix* pLEDMatrix);
	virtual ~TurnOnRunnable();

	void init();
	virtual void run();

protected:
	uint16_t currentOffset = 0;
};

#endif /* TURNONRUNNABLE_H_ */
