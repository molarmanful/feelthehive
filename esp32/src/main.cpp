#include "MyServo.h"

MyServo servos[] = {12, 13, 14, 15, 18, 19};

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
    }
  }

  Serial.print("Connecting");
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_ID, strlen(EAP_ID));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_USERNAME,
                                     strlen(EAP_USERNAME));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD,
                                     strlen(EAP_PASSWORD));
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(EAP_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected ");
  Serial.println(WiFi.localIP());

  for (auto& servo : servos) {
    servo.init();
    Serial.print("servo init: ");
    Serial.println(servo.pin);
  }
}

void loop() {
  // for (auto& servo : servos) servo.servo.write(0);
  while (servos[0].pos < 135) {
    for (auto& servo : servos) servo.inc();
    delay(10);
  }
  while (servos[0].pos > 45) {
    for (auto& servo : servos) servo.dec();
    delay(10);
  }
}