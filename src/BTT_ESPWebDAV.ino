/* Using the WebDAV server
	From windows - 
		Run: \\HOSTNAME\DavWWWRoot
		or Map Network Drive -> Connect to a Website
*/

#include <ESP8266WiFi.h>
#include "ESPWebDAV.h"
#include "WiFiSettings.h"

#define HOSTNAME	"BTT_ESPWebDAV"
#define SERVER_PORT	80
#define SD_CS		5

ESPWebDAV dav;
String statusMessage;
bool initFailed = false;


// ------------------------
void setup() {
// ------------------------
	Serial.begin(115200);
	Serial.println("");
	Serial.println("Starting up");

	WiFi.mode(WIFI_STA);
	WiFi.setPhyMode(WIFI_PHY_MODE_11N);
	WiFi.hostname(HOSTNAME);
	delay(1000);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	// Wait for connection
	while(WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to "); Serial.println(WIFI_SSID);
	Serial.print ("IP address: "); Serial.println(WiFi.localIP());
	Serial.print ("RSSI: "); Serial.println(WiFi.RSSI());
	Serial.print ("Mode: "); Serial.println(WiFi.getPhyMode());
	
	// start the SD DAV server
	if(!dav.init(SERVER_PORT))		{
		statusMessage = "Failed to initialize SD Card";
		Serial.print("ERROR: "); Serial.println(statusMessage);
		initFailed = true;
	}

	Serial.println("WebDAV server started");
}



// ------------------------
void loop() {
// ------------------------
	if(dav.isClientWaiting())	{
		Serial.println("Client connected");
		if(initFailed)
			return dav.rejectClient(statusMessage);

		// call handle if server was initialized properly
		
		dav.initSD(SD_CS, SPI_FULL_SPEED);
		dav.handleClient();
	}
}


