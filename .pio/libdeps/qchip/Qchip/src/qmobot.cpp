#include "qmobot.h"


SSD1306Wire *display;

void begin(bool DisplayEnable, bool SerialEnable, bool BOOT_CTRL, bool PABOOST, long BAND) {

	display = new SSD1306Wire(0x3c, SDA_OLED, SCL_OLED, RST_OLED, GEOMETRY_128_64);
	
	pinMode(RIGHT_SENSORPIN,INPUT);
	pinMode(CENTER_SENSORPIN,INPUT);
	pinMode(LEFT_SENSORPIN,INPUT);

	pinMode(RT, OUTPUT);
	pinMode(RE, INPUT);
	pinMode(LT, OUTPUT);
	pinMode(LE, INPUT);

	pinMode(AIN1, OUTPUT);
	pinMode(AIN2, OUTPUT);
	pinMode(BIN1, OUTPUT);
	pinMode(BIN2, OUTPUT);

	pinMode(PWMA, OUTPUT);
	pinMode(PWMB, OUTPUT);

	if (BOOT_CTRL){
		pinMode(0, INPUT_PULLUP);
		attachInterrupt(0, BOOT_control, RISING);
	}


	// UART
	if (SerialEnable) {
		Serial.begin(115200);
		Serial.flush();
		delay(50);
		Serial.print("Qchip is ready\r\n");
	}

	// OLED
	if (DisplayEnable)
	{
		display->init();
		display->flipScreenVertically();
		display->setFont(ArialMT_Plain_10);
		display->drawString(0, 0, "Qmobot.com");
		display->display();

		if (SerialEnable){
			Serial.print("you can see OLED printed OLED initial done!\r\n");
		}
	}
}

String wifi_connect(char* ssid, char* password){
	WiFi.begin(ssid, password); 
	int i = 0;
	while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
		delay(500);
		i += 1;
		if(i > 20){
			return "Timeout";
		}
	}
	return String(WiFi.localIP()[0]) + String(".") +
           String(WiFi.localIP()[1]) + String(".") +
           String(WiFi.localIP()[2]) + String(".") +
           String(WiFi.localIP()[3]);
}

String wifi_create(char* ssid, char* password){
	WiFi.softAP(ssid, password);
	IPAddress localIP = WiFi.softAPIP();
	return String(localIP[0]) + String(".") +
           String(localIP[1]) + String(".") +
           String(localIP[2]) + String(".") +
           String(localIP[3]);
}

void IRAM_ATTR BOOT_control() {
  Serial.println("Qairly tun!");
  esp_deep_sleep_start();
}

void print(String phrase){
	Serial.print(phrase);
}

void println(String phrase){
	Serial.println(phrase);
}

void show(String phrase, int x, int y){
	display->drawString(x, y, phrase);
	display->display();
}

void erase(){
	display->clear();
}

float battery(){
	float VBAT2 = (float)(analogRead(37))/1024.0; 
	float VBAT = (VBAT2*222)-444;
	return VBAT;
}

void stop(){
	analogWrite(PWMA, 0);
	analogWrite(PWMB, 0);
	digitalWrite(AIN2, LOW);
	digitalWrite(AIN1, LOW);
	digitalWrite(BIN2, LOW);
	digitalWrite(BIN1, LOW);
}

void run(char Motor, int value){
	if(value>255 || value<-255){
		println("ERROR: Value is less than 0 or bigger than 255 for motors");
	}
	else if(Motor!='L' && Motor!='R'){
		println("ERROR: Motor is not R or L");
	}
	else{
		
		if(Motor == 'L'){
			if(value > 0){
				digitalWrite(AIN2, HIGH);
				digitalWrite(AIN1, LOW);
				analogWrite(PWMA, value);
			}
			else if(value < 0){
				digitalWrite(AIN2, LOW);
				digitalWrite(AIN1, HIGH);
				analogWrite(PWMA, -value);
			}
		}
		else{
			if(value > 0){
				digitalWrite(BIN2, HIGH);
				digitalWrite(BIN1, LOW);
				analogWrite(PWMB, value);
			}
			else if(value < 0){
				digitalWrite(BIN2, LOW);
				digitalWrite(BIN1, HIGH);
				analogWrite(PWMB, -value);
			}
		}
	}
}

int echo(char way){
	long duration = 0;
	int distance = 0;

	if(way!='L' && way!='R'){
		println("ERROR: Echo is not R or L");
		return -1;
	}
	else if(way=='R'){
		digitalWrite(RT, LOW);
		delayMicroseconds(2);
		digitalWrite(RT, HIGH);
		delayMicroseconds(10);
		digitalWrite(RT, LOW);
		duration = pulseIn(RE, HIGH);
		distance= duration*0.034/2;
		return distance;
	}
	else if(way=='L'){
		digitalWrite(LT, LOW);
		delayMicroseconds(2);
		digitalWrite(LT, HIGH);
		delayMicroseconds(10);
		digitalWrite(LT, LOW);
		duration = pulseIn(LE, HIGH);
		distance= duration*0.034/2;
		return distance;
	}
	return -1;
}
