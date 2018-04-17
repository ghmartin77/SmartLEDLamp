#ifndef FADEANDSCROLLVISUALIZER_H_
#define FADEANDSCROLLVISUALIZER_H_

#include "Visualizer.h"

class FadeAndScrollVisualizer: public Visualizer {
public:
	FadeAndScrollVisualizer(uint8_t _fsSpeed, uint8_t _fsZoom);
	virtual ~FadeAndScrollVisualizer();

	virtual void start();
	virtual void stop();

	virtual boolean onButtonPressed(uint8_t button);

protected:
	virtual void computeImage();

private:
	int8_t fsSpeed;
	int16_t fsZoom;
	uint16_t fsIndex;
	int16_t* pFsHeight;
	uint8_t* pFsLut;

	void updateFsHeight();
};

#endif /* FADEANDSCROLLVISUALIZER_H_ */
