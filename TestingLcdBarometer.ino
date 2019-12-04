#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>
#include <Ethernet.h>


LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Adafruit_BMP085 sensor = Adafruit_BMP085();

byte mac [] = {0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xE0};
char serverName[] = "192.168.0.14";
int serverPort = 80;
EthernetClient client;


void setup() {

  Serial.begin(9600);
  lcd.begin(16, 2);

  if (!Ethernet.begin(mac)) {
    Serial.println("Error, could not configure with DHCP");
  }
  else {
    Serial.println("Ethernet configured succsessfully.");
    Serial.println("Arduino IP:" + Ethernet.localIP());
  }

  if (!sensor.begin()) {
    Serial.println("Sensor cannot be found");
    lcd.setCursor(0, 0);
    lcd.print("Sensor cannot be found");
    while (1) {}
  }
}

void loop() {
  float temperature = sensor.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Temp.= ");
  lcd.print(temperature);
  lcd.print(" C");

  float pressure = sensor.readPressure() / 101.325;
  pressure *= 0.760;
  lcd.setCursor(0, 1);
  lcd.print("Pres= ");
  lcd.print(pressure);
  lcd.print("mmHg");

  connectToServer(temperature, pressure);

  delay(2000);
}

void connectToServer(float temperature, float pressure) {
  String data = "{";
  data = data + "'temperature':" + temperature + ",";
  data = data + "'pressure':" + pressure + "}";
  Serial.println("data to send:" + data);
  if (client.connect(serverName, serverPort)) {
    Serial.println("Connected to server");

    client.print("POST /test ");
    client.println("HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/javascript;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    
    Serial.println("complete");

  } else {
    Serial.println("connection failed");
  }

  client.stop();
}