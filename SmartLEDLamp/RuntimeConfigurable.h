#ifndef RUNTIME_CONFIGURABLE_H_
#define RUNTIME_CONFIGURABLE_H_

class RuntimeConfigurable {
public:
	virtual ~RuntimeConfigurable() {};
	virtual void readRuntimeConfiguration(int &address) = 0;
	virtual void writeRuntimeConfiguration(int &address) = 0;
};

#endif /* RUNTIME_CONFIGURABLE_H_ */
