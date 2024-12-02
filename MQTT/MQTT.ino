#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AdafruitIO.h> // tiến hành thêm thư viện AdafruitIO.h vào

#define FIREBASE_HOST "https://biot-9f1c0-default-rtdb.firebaseio.com/"
#define FIREBASE_API_KEY "AIzaSyDC0Nyw5aGD2tJ-WtQRWwptDpPP-U5s058"
#define FIREBASE_AUTH "15nrb4s7RicJIW2qkJjo6bTO3N7JP0EsNwzr4u6C"

#define IO_USERNAME  "BinhLuong123"
#define IO_KEY       "aio_mTCV94CwLrdRIxow0904KpWeQgyg"

#define WIFI_SSID "TOTOLINKB"
#define WIFI_PASS "0123456789" 

#include <AdafruitIO_WiFi.h>  // Khai báo thư viện AdafruitIO_WiFi.h để kết nối đến server.
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);  // Gọi hàm kết nối đến server.

AdafruitIO_Feed *digital = io.feed("digital"); // khai báo con trỏ digital để chứ dữ liệu lấy từ feed của server.


#define ONE_WIRE_BUS 25 // Chân kết nối với cảm biến DS18B20
#define LED_PIN 35 // LED on Board là GPIO 2.

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
  if (Firebase.signUp(&FBase_Config, &FBase_Auth, "", "")){
    Serial.println("ok");
  }
  else{
    Serial.printf("%s\n", FBase_Config.signer.signupError.message.c_str());
  }

  // Connect to Firebase
  Firebase.begin(&FBase_Config, &FBase_Auth);
  Firebase.reconnectWiFi(true);

  sensors.begin(); // Khởi tạo cảm biến DS18B20

/////////////////////////////////////////////////////////////

 
  // set led pin as a digital output
  pinMode(LED_PIN, OUTPUT); // Khai báo output.
 
  // start the serial connection
  Serial.begin(115200); 
 
  // wait for serial monitor to open
  while(! Serial);
 
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO"); // tiến hành kết nối đến server.
  io.connect(); // Gọi hàm kết nối
 
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print("."); // Nếu chưa kết nối được đến server sẽ tiến hành xuất ra màn hình đấu "."
    delay(500);
  }
 
  // we are connected
  Serial.println();
  Serial.println(io.statusText()); // Nếu đã kết nối thành công tiến hành xuất ra màn hình trạng thái.
  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  digital->get(); // lấy dữ liệu từ feed 'digital' của server.
  digital->onMessage(handleMessage); // Gọi hàm đọc dữ liệu và tiến hành điều khiển led và xuất ra trạng thái trên màn hình.
}


// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) { // hàm handleMessage để đọc dữ liệu.

 // xuất ra màn hình trạng thái của nút nhấn trên feed vừa đọc được.
  Serial.print("received <- ");
 
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

 // cài đặt trạng thái bật tắt led on board tương ứng với nút nhấn.
  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());
 
}
//////////////////////////////////////////////////////////////////
void loop() {
  
  io.run(); // gọi hàm Run.

  delay(1000);
  
  sensors.requestTemperatures(); // Yêu cầu đọc nhiệt độ
  float t = sensors.getTempCByIndex(0); // Đọc nhiệt độ của cảm biến đầu tiên

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