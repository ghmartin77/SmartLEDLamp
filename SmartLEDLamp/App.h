#ifndef APP_H_
#define APP_H_

#include "LEDMatrix.h"
#include "RuntimeConfigurable.h"

class App: public virtual RuntimeConfigurable {
public:
	App(uint8_t appId, LEDMatrix* pLEDMatrix);
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

	const uint8_t getId() const {
		return id;
	}

protected:
	const uint8_t id;
	const char* name;
	LEDMatrix *pMatrix;
};

#endif /* APP_H_ */
