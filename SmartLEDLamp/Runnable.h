#ifndef RUNNABLE_H_
#define RUNNABLE_H_

#include "LEDMatrix.h"

class Runnable {
public:
	Runnable(LEDMatrix* pLEDMatrix);
	virtual ~Runnable();

	virtual void run() = 0;

protected:
	LEDMatrix *pMatrix;
	unsigned long lastMillis = 0;
};

#endif /* RUNNABLE_H_ */
