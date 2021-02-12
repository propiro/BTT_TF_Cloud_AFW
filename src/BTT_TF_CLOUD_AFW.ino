/*
 * Alternative firmware for BTT TF Cloud devices
 * 
 * 2021 - Albrecht Lohofener (albrechtloh@gmx.de)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ESP8266WiFi.h>
#include "ESPWebDAV.h"
#include "ESPFtpServer.h"
#include "WebOTA.h"
#include "WiFiSettings.h"

#define HOSTNAME	"BTT_TF_CLOUD_AFW"
#define SERVER_PORT	80
#define SD_CS		5

ESPWebDAV dav;
String statusMessage;
bool initFailed = false;

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial


// ------------------------
void setup() {
// ------------------------
	Serial.begin(115200);
	Serial.println("");
	Serial.println("Connecting to WiFi");

	// WiFi.mode(WIFI_STA);
	// WiFi.setPhyMode(WIFI_PHY_MODE_11N);
	// WiFi.hostname(HOSTNAME);
	// delay(1000);

	// WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	// // Wait for connection
	// while(WiFi.status() != WL_CONNECTED) {
	// 	delay(500);
	// 	Serial.print(".");
	// }

	// Serial.println("");
	// Serial.print("Connected to "); Serial.println(WIFI_SSID);
	// Serial.print ("IP address: "); Serial.println(WiFi.localIP());
	// Serial.print ("RSSI: "); Serial.println(WiFi.RSSI());
	// Serial.print ("Mode: "); Serial.println(WiFi.getPhyMode());

	init_wifi(WIFI_SSID, WIFI_PASSWORD, HOSTNAME);
	
	// start the SD DAV server
	if(!dav.init(SERVER_PORT))		{
		statusMessage = "Failed to initialize SD Card";
		Serial.print("ERROR: "); Serial.println(statusMessage);
		initFailed = true;
	}
	Serial.println("WebDAV server started");

	// Start FTP server
	ftpSrv.begin("anonymous","", SD_CS, SPI_FULL_SPEED);    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
	Serial.println("FTP server started");
}

// ------------------------
void loop() {
// ------------------------
	// WebDAY
	if(dav.isClientWaiting())	{
		Serial.println("Client connected");
		if(initFailed)
			return dav.rejectClient(statusMessage);

		// call handle if server was initialized properly
		
		dav.initSD(SD_CS, SPI_FULL_SPEED);
		dav.handleClient();
	}

	// FTP
	ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  

	// Web OTA update
	webota.handle();
}
