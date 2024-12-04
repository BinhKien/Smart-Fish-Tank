#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AdafruitIO.h> // Thêm thư viện Adafruit IO.

#define FIREBASE_HOST "https://biot-9f1c0-default-rtdb.firebaseio.com/"
#define FIREBASE_API_KEY "AIzaSyDC0Nyw5aGD2tJ-WtQRWwptDpPP-U5s058"
#define FIREBASE_AUTH "15nrb4s7RicJIW2qkJjo6bTO3N7JP0EsNwzr4u6C"

#define IO_USERNAME "BinhLuong123"
#define IO_KEY "aio_mTCV94CwLrdRIxow0904KpWeQgyg"

#define WIFI_SSID "TOTOLINKB"
#define WIFI_PASS "0123456789"

#include <AdafruitIO_WiFi.h> // Thư viện AdafruitIO_WiFi.h.
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *digital = io.feed("digital"); // Feed điều khiển LED 1.
AdafruitIO_Feed *digital2 = io.feed("digital2"); // Feed điều khiển LED 2.
AdafruitIO_Feed *digital3 = io.feed("digital3"); // Feed điều khiển LED 3.

#define ONE_WIRE_BUS 25 // Chân kết nối với cảm biến DS18B20.
#define LED1_PIN 12     // LED 1 on Board là GPIO 35.
#define LED2_PIN 13     // LED 2 là GPIO 26.
#define LED3_PIN 14     // LED 3 là GPIO 27.

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

FirebaseData FBase_Data;
FirebaseConfig FBase_Config; 
FirebaseAuth FBase_Auth;     

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Da ket noi WiFi!");

  // Khởi tạo cấu hình Firebase
  FBase_Config.database_url = FIREBASE_HOST;
  FBase_Config.api_key = FIREBASE_API_KEY;
  
  /* Sign up */
  if (Firebase.signUp(&FBase_Config, &FBase_Auth, "", "")) {
    Serial.println("Firebase connected successfully!");
  } else {
    Serial.printf("Firebase error: %s\n", FBase_Config.signer.signupError.message.c_str());
  }

  // Connect to Firebase
  Firebase.begin(&FBase_Config, &FBase_Auth);
  Firebase.reconnectWiFi(true);

  sensors.begin(); // Khởi tạo cảm biến DS18B20.

  // Cài đặt các chân LED làm đầu ra.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Kết nối đến Adafruit IO.
  Serial.print("Connecting to Adafruit IO...");
  io.connect();

  // Chờ kết nối Adafruit IO.
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText()); // Hiển thị trạng thái kết nối.

  // Gắn hàm xử lý cho từng feed.
  digital->onMessage(handleMessage1);
  digital2->onMessage(handleMessage2);
  digital3->onMessage(handleMessage3);
}

// Xử lý khi nhận dữ liệu từ feed 'digital1'.
void handleMessage1(AdafruitIO_Data *data) {
  Serial.print("LED 1: ");
  if (data->toPinLevel() == HIGH) {
    Serial.println("ON");
    digitalWrite(LED1_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(LED1_PIN, LOW);
  }
}

// Xử lý khi nhận dữ liệu từ feed 'digital2'.
void handleMessage2(AdafruitIO_Data *data) {
  Serial.print("LED 2: ");
  if (data->toPinLevel() == HIGH) {
    Serial.println("ON");
    digitalWrite(LED2_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(LED2_PIN, LOW);
  }
}

// Xử lý khi nhận dữ liệu từ feed 'digital3'.
void handleMessage3(AdafruitIO_Data *data) {
  Serial.print("LED 3: ");
  if (data->toPinLevel() == HIGH) {
    Serial.println("ON");
    digitalWrite(LED3_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(LED3_PIN, LOW);
  }
}

void loop() {
  io.run(); // Chạy vòng lặp Adafruit IO.

  delay(1000);
  
  sensors.requestTemperatures(); // Yêu cầu đọc nhiệt độ.
  float t = sensors.getTempCByIndex(0); // Đọc nhiệt độ của cảm biến đầu tiên.

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("°C   ");

  if (Firebase.setFloat(FBase_Data, "nhietdo", t)) {
    Serial.println("Temperature data sent successfully!");
  } else {
    Serial.println("Failed to send temperature data");
    Serial.println(FBase_Data.errorReason());
  }
}
