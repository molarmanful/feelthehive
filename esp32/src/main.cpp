#include "MyServo.h"

websockets::WebsocketsClient client;
MyServo servos[] = {12, 13, 14, 15, 2, 4, 18, 27};
DynamicJsonDocument doc(256);
TaskHandle_t core0;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

bool busy = false;
int vpow = 0;
auto ms = millis();
auto ms1 = millis();

char echo_org_ssl_ca_cert[] PROGMEM =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n"
    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
    "DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n"
    "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
    "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n"
    "AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n"
    "ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n"
    "wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n"
    "LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n"
    "4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n"
    "bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n"
    "sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n"
    "Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n"
    "FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n"
    "SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n"
    "PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n"
    "TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n"
    "SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n"
    "c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n"
    "+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n"
    "ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n"
    "b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n"
    "U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n"
    "MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n"
    "5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n"
    "9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n"
    "WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n"
    "he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n"
    "Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n"
    "-----END CERTIFICATE-----\n";

void disp(String s) {
  display.println(s);
  display.display();
}

void cdisp(String s) {
  display.clearDisplay();
  display.setCursor(0, 0);
  disp(s);
}

void scanWifi() {
  Serial.println("Scanning...");
  cdisp("~scan");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
    cdisp("-scan");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    cdisp("+scan");
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
}

void connWifi() {
  Serial.print("Connecting wifi...");
  cdisp("~wifi");
#ifdef PERSONAL
  WiFi.begin(EAP_SSID, EAP_PASSWORD);
#else
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_ID, strlen(EAP_ID));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_USERNAME,
                                     strlen(EAP_USERNAME));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD,
                                     strlen(EAP_PASSWORD));
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(EAP_SSID);
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected wifi @ ");
  Serial.println(WiFi.localIP());
  cdisp("+wifi");
}

void onMessageCB(websockets::WebsocketsMessage msg) {
  auto data = msg.data();
  deserializeJson(doc, data);
  auto obj = doc.as<JsonObject>();
  vpow = obj["pow"].as<int>();
  Serial.print("pow ");
  Serial.println(vpow);
  if (!busy) {
    cdisp("pow");
    disp(String(vpow));
  }
  ms1 = millis();
  if (vpow > 0) busy = true;
}

void onEventCB(websockets::WebsocketsEvent ev, String data) {
  if (ev == websockets::WebsocketsEvent::ConnectionOpened) {
    Serial.println("socket opened");
  } else if (ev == websockets::WebsocketsEvent::ConnectionClosed) {
    Serial.println("socket closed");
    cdisp("-sock");
    delay(500);
    ESP.restart();
  }
}

void connSock() {
  client = {};
  client.setCACert(echo_org_ssl_ca_cert);
  bool conn = false;
  while (!conn) {
    Serial.println("connecting socket...");
    display.println("~sock");
    conn = client.connect("wss://fth.fly.dev/ws");
  }
  client.onEvent(onEventCB);
  client.onMessage(onMessageCB);
  Serial.println("socket connected");
  cdisp("+sock");
}

void loop0(void* params) {
  while (1) {
    if (busy) {
      int p1 = 6 - min(0, vpow - 50) / 10;
      int p2 = max(1, vpow / 10);
      Serial.print(p1);
      Serial.print(" ");
      Serial.print(p2);
      Serial.println(" busy");
      while (servos[0].pos < 180) {
        for (auto& servo : servos) servo.inc(p2);
        delay(p1);
      }
      while (servos[0].pos > 0) {
        for (auto& servo : servos) servo.dec(p2);
        delay(p1);
      }
      busy = false;
      Serial.println("not busy");
    }
    delay(1);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3d);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  // scanWifi();
  connWifi();
  connSock();

  for (auto& servo : servos) servo.init();

  xTaskCreatePinnedToCore(loop0, "loop0", 1e4, NULL, 1, &core0, 1);
}

void loop() {
  // for (auto& servo : servos) servo.servo.write(0);
  // if (!busy) busy = true;

  client.poll();

  auto cms = millis();
  if (cms - ms >= 500) {
    ms = cms;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("wifi disconnected");
      cdisp("-wifi");
      delay(500);
      ESP.restart();
    }
  }
  if (cms - ms1 >= 10000) {
    ms1 = cms;
    Serial.println("ping");
    cdisp("ping");
    client.send("ping");
  }
}