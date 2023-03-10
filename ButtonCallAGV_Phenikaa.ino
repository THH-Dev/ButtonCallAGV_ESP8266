#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "THH-AGV";
const char* password = "302phuvien";

const int LED_PIN = 2;  // Chân kết nối với đèn báo

WiFiClient client;
HTTPClient http;
bool wifiConnected = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Cấu hình chân kết nối đèn báo là OUTPUT
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  wifiConnected = true;
}

void loop() {

  ReconnectWifi();


  if (digitalRead(0) == 0) {
    delay(150);
    if (digitalRead(0) == 1) {

      http.begin(client, "http://10.68.86.101/api/Remote/Task/DangKyStation?args=5");

      http.addHeader("Content-Type", "application/json");

      // Tạo nội dung yêu cầu(của phenikaa để trống)
      String requestBody = "{}";

      // Gửi yêu cầu POST với nội dung được định dạng là JSON
      int httpResponseCode = http.POST(requestBody);

      // Kiểm tra mã trạng thái HTTP trả về từ máy chủ
      if (httpResponseCode > 0) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.print("Server Response : ");
        Serial.println(response);
        blinkLED(3);  // Server Resonse bật đèn báo


      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      // Đóng kết nối đến máy chủ
      http.end();
    }
  }
}

// Kết nối lại khi bị mất kết nối wifi
void ReconnectWifi() {
  if (wifiConnected) {
    // Kiểm tra kết nối WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection lost");
      wifiConnected = false;
    }
  } else {
    // Kết nối lại WiFi nếu mất kết nối
    Serial.println("Reconnecting to WiFi...");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi reconnected");
    wifiConnected = true;
  }
}

// Tạo hàm nhấp nháy đèn báo
void blinkLED(int count) {
  if (count > 0) {
    // Bật đèn báo
    digitalWrite(LED_PIN, LOW);
    delay(500);
    // Tắt đèn báo
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    blinkLED(count - 1);
  }
}
