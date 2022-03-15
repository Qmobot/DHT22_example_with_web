#include "qmobot.h"


#define STA_SSID "your-ssid"
#define STA_PASS "your-pass"
#define AP_SSID  "esp32"

enum { STEP_BTON, STEP_BTOFF, STEP_STA, STEP_AP, STEP_AP_STA, STEP_OFF, STEP_BT_STA, STEP_END };

void onButton(){
  static uint32_t step = STEP_BTON;
  switch(step){
    case STEP_BTON://BT Only
      Serial.println("** Starting BT");
      btStart();
    break;
    case STEP_BTOFF://All Off
      Serial.println("** Stopping BT");
      btStop();
    break;
    case STEP_STA://STA Only
      Serial.println("** Starting STA");
      WiFi.begin(STA_SSID, STA_PASS);
    break;
    case STEP_AP://AP Only
      Serial.println("** Stopping STA");
      WiFi.mode(WIFI_AP);
      Serial.println("** Starting AP");
      WiFi.softAP(AP_SSID);
    break;
    case STEP_AP_STA://AP+STA
      Serial.println("** Starting STA");
      WiFi.begin(STA_SSID, STA_PASS);
    break;
    case STEP_OFF://All Off
      Serial.println("** Stopping WiFi");
      WiFi.mode(WIFI_OFF);
    break;
    case STEP_BT_STA://BT+STA
      Serial.println("** Starting STA+BT");
      WiFi.begin(STA_SSID, STA_PASS);
      btStart();
    break;
    case STEP_END://All Off
      Serial.println("** Stopping WiFi+BT");
      WiFi.mode(WIFI_OFF);
      btStop();
    break;
    default:
    break;
  }
  if(step == STEP_END){
    step = STEP_BTON;
  } else {
    step++;
  }
  //little debounce
  delay(100);
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("AP Started");
            WiFi.softAPsetHostname(AP_SSID);
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("AP Stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("STA Started");
            WiFi.setHostname(AP_SSID);
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("STA Connected");
            WiFi.enableIpV6();
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.print("STA IPv6: ");
            Serial.println(WiFi.localIPv6());
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("STA IPv4: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("STA Disconnected");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("STA Stopped");
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(0, INPUT_PULLUP);
    WiFi.onEvent(WiFiEvent);
    Serial.print("ESP32 SDK: ");
    Serial.println(ESP.getSdkVersion());
    Serial.println("Press the button to select the next mode");
}

void loop() {
    static uint8_t lastPinState = 1;
    uint8_t pinState = digitalRead(0);
    if(!pinState && lastPinState){
        onButton();
    }
    lastPinState = pinState;
}
