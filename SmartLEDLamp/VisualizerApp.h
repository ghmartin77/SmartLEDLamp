#ifndef VISUALIZERAPP_H_
#define VISUALIZERAPP_H_

#include "App.h"
#include "Visualizer.h"

#define NO_VISUALIZERS 2

class VisualizerApp: public App {
public:
	VisualizerApp(uint8_t appId, LEDMatrix* pLEDMatrix);
	virtual ~VisualizerApp();

	virtual void start();
	virtual void run();
	virtual void update();
	virtual void stop();

	virtual boolean onButtonPressed(uint8_t button);

	virtual void readRuntimeConfiguration(int &address);
	virtual void writeRuntimeConfiguration(int &address);

	void setVisualizer(uint8_t idx, Visualizer* pVisualizer);

private:
	Visualizer* visualizers[NO_VISUALIZERS] = { NULL, NULL };
	unsigned long int lastMillis;

	uint8_t* pImageData[NO_VISUALIZERS] = { NULL, NULL };
};

#endif /* VISUALIZERAPP_H_ */
