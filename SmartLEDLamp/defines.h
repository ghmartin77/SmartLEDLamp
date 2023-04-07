#ifndef DEFINES_H_
#define DEFINES_H_

#define VERSION "0.3"

// Uncomment next line to enable IR code if you don't use an IR receiver
// #define IR_ENABLE

// Uncomment next line to enable Art-Net support
// #define ARTNET_ENABLE

// Uncomment next line to enable MQTT support
// #define MQTT_ENABLE
#define MQTT_SERVER "your.mqtt.broker.ip"
#define MQTT_TOPIC  "your/mqtt/status/topic"

#define UDP_PORT 6789

#define LEDS_WIDTH 4
#define LEDS_HEIGHT 40

#ifdef IR_ENABLE
#define PIN_RECV_IR D3
#endif

#define PIN_CLOCK D1
#define PIN_DATA D2

#define STRIP_RGB_ORDER RBG
#define STRIP_CHIPSET WS2801

#define BTN_BRIGHTER 1
#define BTN_DARKER 2
#define BTN_PAUSE 3
#define BTN_POWER 4
#define BTN_R 5
#define BTN_G 6
#define BTN_B 7
#define BTN_W 8
#define BTN_RED_UP 25
#define BTN_GREEN_UP 26
#define BTN_BLUE_UP 27
#define BTN_QUICK 28
#define BTN_RED_DOWN 29
#define BTN_GREEN_DOWN 30
#define BTN_BLUE_DOWN 31
#define BTN_SLOW 32
#define BTN_DIY1 33
#define BTN_DIY2 34
#define BTN_DIY3 35
#define BTN_AUTO 36
#define BTN_DIY4 37
#define BTN_DIY5 38
#define BTN_DIY6 39
#define BTN_FLASH 40
#define BTN_JUMP3 41
#define BTN_JUMP7 42
#define BTN_FADE3 43
#define BTN_FADE7 44

#define MAGIC_MARKER 0xCA
#define EEPROM_SIZE 1024

#endif /* DEFINES_H_ */
