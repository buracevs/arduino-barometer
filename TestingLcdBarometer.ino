#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>


LiquidCrystal_I2C lcd(0x3F, 2,1,0,4,5,6,7,3, POSITIVE);
Adafruit_BMP085 sensor = Adafruit_BMP085();
 
void setup() {
  
Serial.begin(9600);
lcd.begin(16,2);
//pinMode(ledPort, OUTPUT);
  if(!sensor.begin()){
    Serial.println("Sensor cannot be found");
    lcd.setCursor(0, 0);  
    lcd.print("Sensor cannot be found");
    while(1){}
  }


}

void loop() {
/*Serial.println("Data from Arduino Sensor "); */
 /*lcd.setCursor(0, 0);
 lcd.print("Data from Arduino Sensor ");
 delay(3000);*/

  Serial.print("Temperature = ");
  Serial.print(sensor.readTemperature());
  Serial.println("*C");

  lcd.setCursor(0, 0); 
  lcd.print("Temp.= ");
  lcd.print(sensor.readTemperature());
  lcd.print(" C");

  float pressure = sensor.readPressure()/101.325;
  pressure*=0.760;
  lcd.setCursor(0, 1);
  lcd.print("Pres= "); 
  lcd.print(pressure);
  lcd.print("mmHg");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" mmHg");
  
  delay(2000);


  
}
