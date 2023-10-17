#include <WiFi.h>
#include <fauxmoESP.h>

#define WIFI_SSID		""
#define WIFI_PASSWORD	""
#define WIFI_LED		2

byte relayPins []		= {};
char* relayNames []		= {};
bool relayStates []		= {};
#define N_RELAYS sizeof(relayPins)

int pumpPin = 13
bool pumpState = false

fauxmoESP fauxmo;

void setupWifi() {
	Serial.printf("Trying to connect to %s...", WIFI_SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	digitalWrite(WIFI_LED, HIGH);
	while(WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	digitalWrite(WIFI_LED, LOW);
	Serial.printf("Connected with IP %s", WiFi.localIP());
}

void initializePins() {
	Serial.println("Initializing pins...");
	for (int n = 0; n < N_RELAYS; n++)  {
		pinMode(relayPins[n], OUTPUT);
		digitalWrite(relayPins[n], HIGH);
	}
}

int findRelay(char* relayName) {
	for (n = 0; n < N_RELAYS; n++){
		if(relayNames[n] == relayName){
			return n;
		}
	}
	return -1;
}

void handlePump(){
	for (int n = 0; n < N_RELAYS; n++)  {
		if(relayStates[n] == true) {
			Serial.printf("Relay %s is on", relayNames[n]);
			Serial.println("Turn on pump");
			return;
		}
	}
	Serial.println("Turn off pump");
}

void addAlexaDevices() {
	Serial.println("Adding Alex devices...");
	for (int n = 0; n < N_RELAYS; n++)  {
		fauxmo.addDevice(relayNames[n]);
		relayStates[n] = false
	}

	fauxmo.setPort(80);
	fauxmo.enable(true);
	fauxmo.onSetState([](unsigned char id, const char * relayName, bool state, unsigned char value) {
		Serial.printf("Relay %s state is %s\n", relayName, state ? "ON" : "OFF");
		relayStates[findRelay(relayName)] = state;
		handlePump();
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
		digitalWrite(WIFI_LED, HIGH);
		setupWifi();
	} else {
		fauxmo.handle();
	}
}
