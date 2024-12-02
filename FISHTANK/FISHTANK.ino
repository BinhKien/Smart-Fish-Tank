#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "https://biot-9f1c0-default-rtdb.firebaseio.com/"
#define FIREBASE_API_KEY "AIzaSyDC0Nyw5aGD2tJ-WtQRWwptDpPP-U5s058"
#define FIREBASE_AUTH "15nrb4s7RicJIW2qkJjo6bTO3N7JP0EsNwzr4u6C"

#define WIFI_SSID "TOTOLINKB"
#define WIFI_PASSWORD "0123456789"

#define ONE_WIRE_BUS 25 // Chân kết nối cảm biến DS18B20
#define LED_PIN1 14       // Chân điều khiển LED
#define LED_PIN2 12       // Chân điều khiển LED
#define LED_PIN3 13       // Chân điều khiển LED

OneWire oneWire(ONE_WIRE_BUS);                // Khởi tạo giao tiếp 1-Wire
DallasTemperature sensors(&oneWire);         // Khởi tạo đối tượng DS18B20
FirebaseData FBase_Data;                     
FirebaseConfig FBase_Config;                 
FirebaseAuth FBase_Auth;                     

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nDa ket noi WiFi!");

  // Cấu hình Firebase
  FBase_Config.database_url = FIREBASE_HOST;
  FBase_Config.api_key = FIREBASE_API_KEY;
  if (Firebase.signUp(&FBase_Config, &FBase_Auth, "", "")) {
    Serial.println("Firebase setup OK");
  } else {
    Serial.printf("Firebase signup error: %s\n", FBase_Config.signer.signupError.message.c_str());
  }

  Firebase.begin(&FBase_Config, &FBase_Auth);
  Firebase.reconnectWiFi(true);

  // Bắt đầu giao tiếp DS18B20
  sensors.begin();
  
  // Cấu hình LED
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
}

void loop() {
  delay(10000);

  // Đọc nhiệt độ từ DS18B20
  sensors.requestTemperatures();          // Gửi lệnh yêu cầu đọc nhiệt độ
  float t = sensors.getTempCByIndex(0);   // Lấy giá trị nhiệt độ (chỉ số 0 nếu chỉ có một cảm biến)
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Gửi nhiệt độ lên Firebase
  if (Firebase.setFloat(FBase_Data, "nhietdo", t)) {
    Serial.println("Temperature data sent successfully!");
  } else {
    Serial.println("Failed to send temperature data");
    Serial.println(FBase_Data.errorReason());
  }
  // Đọc trạng thái thiết bị từ Firebase và điều khiển LED
  int ledState1, ledState2, ledState3, ledState4;

  // LED 1
  if (Firebase.getInt(FBase_Data, "thietbi1/den")) {
    if (FBase_Data.dataType() == "int") {
      ledState1 = FBase_Data.intData();
      Serial.print("Trang thai den 1: ");
      Serial.println(ledState1);
      digitalWrite(LED_PIN1, ledState1 ? HIGH : LOW);
    }
  } else {
    Serial.println("Khong doc duoc Firebase cho den 1");
    Serial.println(FBase_Data.errorReason());
  }

  // LED 2
  if (Firebase.getInt(FBase_Data, "thietbi2/oxi")) {
    if (FBase_Data.dataType() == "int") {
      ledState2 = FBase_Data.intData();
      Serial.print("Trang thai oxi: ");
      Serial.println(ledState2);
      digitalWrite(LED_PIN2, ledState2 ? HIGH : LOW);
    }
  } else {
    Serial.println("Khong doc duoc Firebase cho oxi");
    Serial.println(FBase_Data.errorReason());
  }

  // LED 3
  if (Firebase.getInt(FBase_Data, "thietbi3/bom")) {
    if (FBase_Data.dataType() == "int") {
      ledState3 = FBase_Data.intData();
      Serial.print("Trang thai bom: ");
      Serial.println(ledState3);
      digitalWrite(LED_PIN3, ledState3 ? HIGH : LOW);
    }
  } else {
    Serial.println("Khong doc duoc Firebase cho bom");
    Serial.println(FBase_Data.errorReason());
  }
}
