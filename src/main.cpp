#include <WiFi.h>
#include <Espalexa.h>


// Wifi Credentials
#define WIFI_SSID       ""
#define WIFI_PASSWORD 	""

// Configure PINs
#define WIFI_LED 		    2
#define RELAY_1         23

// Configure device names
#define RELAY_1_NAME  	""
#define RELAY_2_NAME	  ""
#define RELAY_3_NAME	  ""
#define RELAY_4_NAME	  ""
#define RELAY_5_NAME	  ""
#define RELAY_6_NAME	  ""
#define RELAY_7_NAME	  ""
#define RELAY_8_NAME  	""

// Callback functions
void relayStateChanged();

void relayStateChanged() {
	Serial.println("Relay state changed");
}

void setupWifi() {
	Serial.printf("Trying to connect to %s", WIFI_SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while(WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("Connected with IP %s", WiFi.localIP());
}

void connectDevices() {
	Serial.println("Adding Alex devices");
	espalexa.addDevice(RELAY_1_NAME, relayStateChanged);
	espalexa.begin();
}


void setup() {
	Serial.begin(115200);
	
	pinMode(WIFI_LED, OUTPUT);
	digitalWrite(WIFI_LED, HIGH);
	setupWifi();
	digitalWrite(WIFI_LED, HIGH);

  pinMode(RELAY_1_NAME, OUTPUT);
	connectDevices();
}
