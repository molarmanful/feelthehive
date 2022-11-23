#include "MyServo.h"

websockets::WebsocketsClient client;
MyServo servos[] = {12, 13, 14, 15, 2, 4, 18, 27};
DynamicJsonDocument doc(256);
bool ready = true;
int vpow = 0;

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

void scanWifi() {
  Serial.println("Scanning...");
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
}

void connWifi() {
  Serial.print("Connecting wifi...");
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
  Serial.print("\nConnected wifi @ ");
  Serial.println(WiFi.localIP());
}

void connSock() {
  client.setCACert(echo_org_ssl_ca_cert);
  Serial.println("connecting socket...");
  bool conn = false;
  while (!conn) {
    Serial.println("ws connect failed, trying again...");
    conn = client.connect("wss://fth.fly.dev/ws");
  }
  Serial.println("socket connected");
}

void onEventCB(websockets::WebsocketsEvent ev, String data) {
  if (ev == websockets::WebsocketsEvent::ConnectionOpened) {
    Serial.println("socket opened");
  } else if (ev == websockets::WebsocketsEvent::ConnectionClosed) {
    Serial.println("socket closed");
    connSock();
  }
}

void onMessageCB(websockets::WebsocketsMessage msg) {
  if (ready) {
    auto data = msg.data();
    Serial.println(data);
    deserializeJson(doc, data);
    auto obj = doc.as<JsonObject>();
    vpow = obj["pow"].as<int>();
    if (vpow > 0) {
      ready = false;
      while (servos[0].pos < 180) {
        for (auto& servo : servos) servo.inc();
        delay(10);
      }
      while (servos[0].pos > 0) {
        for (auto& servo : servos) servo.dec();
        delay(10);
      }
      ready = true;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  scanWifi();
  connWifi();
  connSock();

  client.onEvent(onEventCB);
  client.onMessage(onMessageCB);

  for (auto& servo : servos) {
    servo.init();
    Serial.print("servo init: ");
    Serial.println(servo.pin);
  }
}

void loop() {
  // for (auto& servo : servos) servo.servo.write(0);
  client.poll();
}