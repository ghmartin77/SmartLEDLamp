#ifndef LEDLAMP_H_
#define LEDLAMP_H_

#include "defines.h"
#include "Runnable.h"

extern boolean isOn;
extern Runnable* pCurrentRunnable;

#ifdef MQTT_ENABLE
#include <PubSubClient.h>
extern PubSubClient mqttClient;
#endif // MQTT_ENABLE


#endif /* LEDLAMP_H_ */
