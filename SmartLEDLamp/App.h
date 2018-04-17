#ifndef APP_H_
#define APP_H_

#include "LEDMatrix.h"

class App {
public:
	App(LEDMatrix* pLEDMatrix);
	virtual ~App();

	virtual void start() {
	}

	virtual void stop() {
	}

	virtual boolean onButtonPressed(uint8_t button) {
		return false;
	}

	virtual void run() = 0;
	virtual void update() = 0;

	const char* getName() const {
		return name;
	}

protected:
	const char* name;
	LEDMatrix *pMatrix;
};

#endif /* APP_H_ */
