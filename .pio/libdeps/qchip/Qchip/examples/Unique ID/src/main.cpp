#include <Arduino.h>

uint32_t chipId = 0;

void IRAM_ATTR BOOT_control() {
  Serial.println("Qairly tun!");
  esp_deep_sleep_start();
}


void setup() {
	// begin();
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(25, HIGH);
  attachInterrupt(0, BOOT_control, RISING);
  Serial.println("Finished Setup");
}


void loop() {
	// for(int i=0; i<17; i=i+8) {
	//   chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	// }

	// Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
	// Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  // Serial.print("Chip ID: "); Serial.println(chipId);
  
	// delay(3000);

  Serial.println("I am awake");
  delay(500);
  // show("QMOBOT.COM");
}