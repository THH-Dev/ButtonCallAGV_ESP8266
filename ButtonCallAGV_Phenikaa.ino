#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "THH_KINHDOANH";
const char* password = "302phuvien";

const int LED_PIN = 2; // Chân kết nối với đèn báo

WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(LED_PIN, OUTPUT); // Cấu hình chân kết nối đèn báo là OUTPUT
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
  
}

void loop() {
  // Kết nối đến máy chủ
  http.begin(client, "http://192.168.100.127:8182/rcms/services/rest/hikRpcService/genAgvSchedulingTask");

  if (digitalRead(0) == 0) {
    delay(150);
    if (digitalRead(0) == 1) {

      // Tạo đối tượng JSON và chuyển đổi thành chuỗi JSON
      StaticJsonDocument<200> doc;
      doc["reqTime"] = "";
      doc["taskTyp"] = "nhiemvu1";
      JsonArray positionCodePath = doc.createNestedArray("positionCodePath");
      JsonObject positionCode1 = positionCodePath.createNestedObject();
      positionCode1["positionCode"] = "kho2";
      positionCode1["type"] = "00";
      JsonObject positionCode2 = positionCodePath.createNestedObject();
      positionCode2["positionCode"] = "kho1";
      positionCode2["type"] = "00";
      doc["podCode"] = "100100";
      doc["agvCode"] = "8267";
      doc["taskCode"] = "1234567891";
      String jsonString;
      serializeJson(doc, jsonString);

      // Gửi yêu cầu POST và chuỗi JSON đến máy chủ
      int httpResponseCode = http.POST(jsonString);

      // Kiểm tra mã trạng thái HTTP trả về từ máy chủ
      if (httpResponseCode > 0) {
        
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.print("Server Response : ");
        Serial.println(response);
        blinkLED(5); // Server Resonse bật đèn báo
        

      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      // Đóng kết nối đến máy chủ
      http.end();
    }
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