#include <Arduino.h>
#include <ArtnetWifi.h>
#include <Esp.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <include/wl_definitions.h>
#include <IPAddress.h>
#include <pins_arduino.h>
#include <cstdint>
#include <ArduinoOTA.h>
#include <IRremoteESP8266.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>

#include "defines.h"
#include "Log.h"
#include "LEDMatrix.h"
#include "App.h"
#include "VisualizerApp.h"
#include "FadeAndScrollVisualizer.h"
#include "FireVisualizer.h"
#include "DemoReelVisualizer.h"
#include "VUMeterVisualizer.h"
#include "NoiseWithPaletteVisualizer.h"

ESP8266WebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t socketNumber = 0;
unsigned long messageNumber;

LEDMatrix matrix(PIN_DATA, PIN_CLOCK, LEDS_WIDTH, LEDS_HEIGHT);

ArtnetWifi artnet;
IRrecv irrecv(PIN_RECV_IR);

boolean isOn = false;
boolean isPlaying = true;

App* pCurrentApp = NULL;
VisualizerApp* pVisApp1;
VisualizerApp* pVisApp2;
VisualizerApp* pVisApp3;
VisualizerApp* pVisApp4;
VisualizerApp* pVisApp5;

int sensorPin = A0;
long sensorValue = 0;

long lmillis = 0;
uint8_t lastButton = 0;
int r = 255, g = 255, b = 255;
float brightness = 1.0f;

float val = 0.0f;
float maxVal = 0.0f;
float minVal = 2.0f;
float curVal = 0.0f;

long lastSensorBurstRead = 0;
long lastMillis = 0;

void onButton(uint8_t btn);
void update();
void switchApp(App* pApp);

long irCodes[] = { 0xff3ac5, 0xffba45, 0xff827d, 0xff02fd, 0xff1ae5, 0xff9a65,
		0xffa25d, 0xff22dd, 0xff2ad5, 0xffaa55, 0xff926d, 0xff12ed, 0xff0af5,
		0xff8a75, 0xffb24d, 0xff32cd, 0xff38c7, 0xffb847, 0xff7887, 0xfff807,
		0xff18e7, 0xff9867, 0xff58a7, 0xffd827, 0xff28d7, 0xffa857, 0xff6897,
		0xffe817, 0xff08f7, 0xff8877, 0xff48b7, 0xffc837, 0xff30cf, 0xffb04f,
		0xff708f, 0xfff00f, 0xff10ef, 0xff906f, 0xff50af, 0xffd02f, 0xff20df,
		0xffa05f, 0xff609f, 0xffe01f };

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence,
		uint8_t* data) {

	int pixel = 0;
	uint16_t x = 0, y = 0;
	for (int i = 0; i < length; i += 3) {
		matrix.setPixel(x, y, data[i], data[i + 1], data[i + 2]);
		if (++x >= LEDS_WIDTH) {
			++y;
			x = 0;
		}
	}

	matrix.update();
}

void connectToWiFi() {
	WiFi.begin();
	ESP.wdtDisable();
	WiFiManager wifiManager;
	wifiManager.setDebugOutput(true); // without this WDT restarts ESP
	wifiManager.autoConnect("SmartLampAP");
	ESP.wdtEnable(0);
}

void handleAction() {
	String act = server.arg("act");
	if (act.length() != 0) {
		if (act == "off") {
			isOn = false;
			update();
		} else if (act == "on") {
			isOn = true;
			update();
		}
	}

	String btn = server.arg("btn");
	if (btn.length() != 0) {
		long btnNo = btn.toInt();

		if (btnNo >= 0) {
			onButton((uint8_t) btnNo);
		}
	}

	String brightness = server.arg("brightness");
	if (brightness.length() != 0) {
		long bghtness = brightness.toInt();

		if (bghtness >= 1 && bghtness <= 100) {
			matrix.setBrightness((float) (bghtness / 100.0));
			update();
		}
	}

	server.send(200, "text/plain", "OK");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload,
		size_t lenght) {
	socketNumber = num;

	switch (type) {
	case WStype_DISCONNECTED:
		Serial.printf("[%u] Disconnected!\n", num);
		break;
	case WStype_CONNECTED:
		Serial.printf("[%u] Connected from ", num);
		Serial.println(webSocket.remoteIP(num));
		webSocket.sendTXT(num, "Connected");
		break;
	case WStype_TEXT:
		//
		// Format of message to process
		// # MESSAGE_NUMBER brightness calibRed calibGreen calibBlue
		// other formats are ignored
		//
		if (payload[0] == '#') {
			char* token = strtok((char*) &payload[2], " ");
			messageNumber = (unsigned long) strtol(token, '\0', 10);
			token = strtok('\0', " ");
			double brightness = (double) strtod(token, '\0');
			matrix.setBrightness(brightness);

			token = strtok('\0', " ");
			double calibrationRed = (double) strtod(token, '\0');
			token = strtok('\0', " ");
			double calibrationGreen = (double) strtod(token, '\0');
			token = strtok('\0', " ");
			double calibrationBlue = (double) strtod(token, '\0');

			matrix.setCalibration(calibrationRed, calibrationGreen,
					calibrationBlue);

			matrix.update();
		} else if (payload[0] == '!') {
			char* token = strtok((char*) &payload[2], " ");
			uint8_t red = (unsigned long) strtol(token, '\0', 10);
			token = strtok('\0', " ");
			uint8_t green = (unsigned long) strtol(token, '\0', 10);
			token = strtok('\0', " ");
			uint8_t blue = (unsigned long) strtol(token, '\0', 10);

			r = red;
			g = green;
			b = blue;
			switchApp(NULL);

			update();
		} else {
			Serial.printf("[%u] get Text: %s\n", num, payload);
		}
		break;
	default:
		Serial.println("Case?");
		break;
	}
}

void startWebServer() {
	server.on("/action/", handleAction);
	server.begin();

	server.serveStatic("/", SPIFFS, "/", "no-cache");

	Serial.println("HTTP server started");

	webSocket.onEvent(webSocketEvent);
	webSocket.begin();

	Serial.println("WebSocket server started");
}

void setupOTA() {
	ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname("LEDLampESP8266");

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR)
		Serial.println("Auth Failed");
		else
		if (error == OTA_BEGIN_ERROR)
		Serial.println("Begin Failed");
		else
		if (error == OTA_CONNECT_ERROR)
		Serial.println("Connect Failed");
		else
		if (error == OTA_RECEIVE_ERROR)
		Serial.println("Receive Failed");
		else
		if (error == OTA_END_ERROR)
		Serial.println("End Failed");
	});
	ArduinoOTA.begin();
}

void switchApp(App* pApp) {
	if (pCurrentApp) {
		pCurrentApp->stop();
	}
	if (pApp) {
		pApp->start();
	}

	pCurrentApp = pApp;
}

void setup() {
	delay(1000);

	Logger.begin();
	Logger.info("\n\n\n\n\rStarting LED Lamp");
	Logger.info("Free Sketch Space: %i", ESP.getFreeSketchSpace());

	// Turn off blue status LED
	digitalWrite(D4, LOW);

	SPIFFS.begin();

	matrix.init();
	matrix.clear();
	//matrix.setCalibration(1.0f, 0.88f, 0.46f);
	matrix.setCalibration(0.62f, 1.0f, 0.76f);

	wifi_set_sleep_type(NONE_SLEEP_T);// see https://github.com/esp8266/Arduino/issues/2070

	connectToWiFi();
	setupOTA();

	matrix.clear();

	startWebServer();

	artnet.setArtDmxCallback(onDmxFrame);
	artnet.begin();

	matrix.clear();

	pVisApp1 = new VisualizerApp(&matrix);
	pVisApp1->setVisualizer(0, new FadeAndScrollVisualizer(1, 20));

	pVisApp2 = new VisualizerApp(&matrix);
	pVisApp2->setVisualizer(0, new FireVisualizer());

	pVisApp3 = new VisualizerApp(&matrix);
	pVisApp3->setVisualizer(0, new DemoReelVisualizer());

	pVisApp4 = new VisualizerApp(&matrix);
	pVisApp4->setVisualizer(0, new VUMeterVisualizer(&curVal));

	pVisApp5 = new VisualizerApp(&matrix);
	pVisApp5->setVisualizer(0, new NoiseWithPaletteVisualizer());

	switchApp(NULL);

//	matrix.setBrightness(0.1f);
//	switchApp(pVisApp1);
//	isOn = true;

	irrecv.enableIRIn(); // Start the receiver

	update();

	pinMode(sensorPin, INPUT);
}

void update() {
	if (isOn) {
		if (pCurrentApp) {
			pCurrentApp->update();
		} else {
			matrix.fill(r, g, b);
		}
	} else {
		matrix.clear();
	}
	matrix.update();
}

void onButton(uint8_t btn) {
	Logger.debug("Button '%i' pressed", btn);

	if (btn == BTN_POWER) {
		isOn = !isOn;
		update();
		delay(200);
	}

	if (!isOn) {
		return;
	}

	if (pCurrentApp) {
		if (pCurrentApp->onButtonPressed(btn))
			return;
	}

	switch (btn) {
	case BTN_BRIGHTER:
		brightness += 0.05f;
		if (brightness > 1.0f)
			brightness = 1.0f;
		matrix.setBrightness(brightness);
		matrix.update();
		break;
	case BTN_DARKER:
		brightness -= 0.05f;
		if (brightness < 0.05f)
			brightness = 0.05f;
		matrix.setBrightness(brightness);
		matrix.update();
		break;
	case BTN_PAUSE:
		isPlaying = !isPlaying;
		delay(200);
		break;

	case BTN_R:
		switchApp(NULL);
		g = b = 0;
		r = 0xff;
		update();
		break;
	case BTN_G:
		switchApp(NULL);
		r = b = 0;
		g = 0xff;
		update();
		break;
	case BTN_B:
		switchApp(NULL);
		r = g = 0;
		b = 0xff;
		update();
		break;
	case BTN_W:
		switchApp(NULL);
		r = g = b = 0xff;
		update();
		break;

	case 9:
		switchApp(NULL);
		r = 0xf5;
		g = 0x28;
		b = 0x0a;
		update();
		break;
	case 10:
		switchApp(NULL);
		r = 0x00;
		g = 0xff;
		b = 0x14;
		update();
		break;
	case 11:
		switchApp(NULL);
		r = 0x19;
		g = 0x19;
		b = 0xff;
		update();
		break;
	case 12:
		switchApp(NULL);
		r = 0xf5;
		g = 0x69;
		b = 0x1e;
		update();
		break;

	case 13:
		switchApp(NULL);
		r = 0xff;
		g = 0x28;
		b = 0x0f;
		update();
		break;
	case 14:
		switchApp(NULL);
		r = 0x00;
		g = 0x7d;
		b = 0x69;
		update();
		break;
	case 15:
		switchApp(NULL);
		r = 0x00;
		g = 0x00;
		b = 0x14;
		update();
		break;
	case 16:
		switchApp(NULL);
		r = 0xf5;
		g = 0x69;
		b = 0x1e;
		update();
		break;

	case 17:
		switchApp(NULL);
		r = 0xf5;
		g = 0x28;
		b = 0x0a;
		update();
		break;
	case 18:
		switchApp(NULL);
		r = 0x00;
		g = 0x46;
		b = 0x37;
		update();
		break;
	case 19:
		switchApp(NULL);
		r = 0xb4;
		g = 0x00;
		b = 0x69;
		update();
		break;
	case 20:
		switchApp(NULL);
		r = 0x4b;
		g = 0x55;
		b = 0xff;
		update();
		break;

	case 21:
		switchApp(NULL);
		r = 0xfa;
		g = 0xec;
		b = 0x00;
		update();
		break;
	case 22:
		switchApp(NULL);
		r = 0x00;
		g = 0x23;
		b = 0x0a;
		update();
		break;
	case 23:
		switchApp(NULL);
		r = 0xe6;
		g = 0x00;
		b = 0x37;
		update();
		break;
	case 24:
		switchApp(NULL);
		r = 0x4b;
		g = 0x55;
		b = 0xff;
		update();
		break;

	case BTN_RED_UP:
		r += 5;
		if (r > 255)
			r = 255;
		update();
		break;
	case BTN_RED_DOWN:
		r -= 5;
		if (r < 0)
			r = 0;
		update();
		break;
	case BTN_GREEN_UP:
		g += 5;
		if (g > 255)
			g = 255;
		update();
		break;
	case BTN_GREEN_DOWN:
		g -= 5;
		if (g < 0)
			g = 0;
		update();
		break;
	case BTN_BLUE_UP:
		b += 5;
		if (b > 255)
			b = 255;
		update();
		break;
	case BTN_BLUE_DOWN:
		b -= 5;
		if (b < 0)
			b = 0;
		update();
		break;

	case BTN_FLASH:
		switchApp(pVisApp5);
		break;

	case BTN_JUMP3:
		switchApp(pVisApp1);
		break;
	case BTN_JUMP7:
		switchApp(pVisApp2);
		break;
	case BTN_FADE3:
		switchApp(pVisApp3);
		break;
	case BTN_FADE7:
		switchApp(pVisApp4);
		break;
	}

}

void readAnalogPeek() {
	maxVal = -10.0f;
	minVal = 10.0f;
//	String message = "# " + String(messageNumber) + " ";
	for (int i = 0; i < 300; ++i) {
		sensorValue = analogRead(sensorPin);
//		if (i & 1) {
//			message = message + String(sensorValue) + ";";
//		}
		float newVal = sensorValue / 1024.0f;
		if (newVal > maxVal)
			maxVal = newVal;

		if (newVal < minVal)
			minVal = newVal;

		if (maxVal - minVal > curVal) {
			curVal = maxVal - minVal;
		}
	}
//	message[message.length() - 1] = '\0';
//	webSocket.sendTXT(socketNumber, message);
	lastSensorBurstRead = millis();
}

void loop() {
	long currentMillis = millis();

//	if (currentMillis > lastSensorBurstRead + 6 /*5*/) {
//		readAnalogPeek();
//	}

	ArduinoOTA.handle();
	server.handleClient();
	webSocket.loop();
	Logger.loop();

	if (isOn && isPlaying) {
		if (pCurrentApp) {
			pCurrentApp->run();
		} else {
			artnet.read();
		}
	}

	decode_results results;
	uint8_t button = 0;

	if (currentMillis > lmillis + 200) {
		lastButton = 0;
	}

	if (irrecv.decode(&results)) {
		if (results.value == 0xffffffff) {
			lmillis = currentMillis;
		} else if ((results.value & 0xff000000) == 0) {

			for (int i = 0; i < sizeof(irCodes) / sizeof(irCodes[0]); ++i) {
				if (irCodes[i] == results.value) {
					button = i + 1;
					lastButton = button;
					lmillis = currentMillis;
					break;
				}
			}

		}
		if (lastButton)
			onButton(lastButton);
		irrecv.resume(); // Receive the next value
	}
}
