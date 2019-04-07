//file->preferences->additional bord urls
//tools->Board->BoardManager->install "esp sressif system"
//select firebeetles-esp32esp 
//https://dl.espressif.com/dl/package_esp32_index.json
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  Serial.println("Hello s from DFRobot ESP-WROOM-32A");
  delay(1000);
}
