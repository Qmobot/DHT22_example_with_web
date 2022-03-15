#include <qmobot.h>
#include "DHT.h"
#include <WiFi.h>

#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


String EL = "OFF";
String ER = "OFF";
String LS = "OFF";
String RS = "OFF";
String CS = "OFF";
String VBAT = "OFF";
String MLs = "OFF";
String MRs = "OFF";
String LEDs = "OFF";
uint32_t chipId = 0;
String name2;


const char* ssid = "Qmobot";
const char* password = "12345678";
WiFiServer server(80);
String header;

float temp = 0;
float hum = 0;


void setup() { // This function runs once
	begin(true, true); // Function to initialize Qmobot
	dht.begin();
	WiFi.softAP(ssid, password);
  	IPAddress IP = WiFi.softAPIP();
	server.begin();
	erase();
	show(IP.toString());
}


void CheckVals(){
	hum = dht.readHumidity();
  	// Read temperature as Celsius (the default)
  	temp = dht.readTemperature();
	if (isnan(hum) || isnan(temp)) {
		hum = -1;
		temp = -1;
    	Serial.println(F("Failed to read from DHT sensor!"));
    }
	Serial.print("Temp: ");
	Serial.println(temp);
	Serial.print("Hum: ");
	Serial.println(hum);
}

void loop() { // This function runs in loop

			CheckVals();
	WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
			if(hum == -1){
				client.println("<body><h1>Sensor is starting, wait...</h1></body>");
			}
			else{
				client.println("<body><h1>Qmobot temperature sensor</h1>");
				client.println("<p>Temperature: </p>");   
				client.println("<p><button class=\"button\">" + String(temp) + "</button></p>");
				client.println("<body><h1>Qmobot humidity sensor</h1>");
				client.println("<p>Humidity: </p>");   
				client.println("<p><button class=\"button\">" + String(hum) + "</button></p>");
				client.println("</body></html>");
			}

            
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
