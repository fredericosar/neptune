#include <WiFi.h>
#include <fauxmoESP.h>

// Wifi Configution
#define WIFI_SSID		""
#define WIFI_PASSWORD	""
#define WIFI_LED		2

// Configure pins
byte relayPins [] 		= {};
char* relayNames [] 	= {};
#define N_RELAYS sizeof(relayPins)

// Enum for easy controlling
enum {Off = HIGH, On = LOW};

fauxmoESP fauxmo;

void setupWifi() {
	Serial.printf("Trying to connect to %s...", WIFI_SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	digitalWrite(WIFI_LED, Off);
	while(WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	digitalWrite(WIFI_LED, On);
	Serial.printf("Connected with IP %s", WiFi.localIP());
}

void initializePins() {
	Serial.println("Initializing pins...");
	for (int n = 0; n < N_RELAYS; n++)  {
		pinMode(relayPins[n], OUTPUT);
		digitalWrite(relayPins[n], Off);
    }
}

void addAlexaDevices() {
	Serial.println("Adding Alex devices...");
	for (int n = 0; n < N_RELAYS; n++)  {
		fauxmo.addDevice(relayNames[n]);
    }

    fauxmo.setPort(80);
    fauxmo.enable(true);
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("Relay %s state is %s\n", device_name, state ? "ON" : "OFF");
    });
}


void setup() {
	Serial.begin(115200);
	pinMode(WIFI_LED, OUTPUT);
	setupWifi();
	initializePins();
	addAlexaDevices();
}

void loop() {
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("We lost WiFi connection. Retry.");
		digitalWrite(WIFI_LED, Off);
		setupWifi();
	} else {
		fauxmo.handle();
	}
}
