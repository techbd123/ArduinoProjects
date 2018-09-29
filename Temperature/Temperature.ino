#include <LiquidCrystal.h>
using namespace std;

LiquidCrystal lcd(7,6,5,4,3,2);

int sensorPin = 0;
void setup()
{
  Serial.begin(9600); 
  lcd.begin(16,2);
  lcd.clear();4
}
 
void loop()    
{
 int reading = analogRead(sensorPin);  
 
 Serial.print("reading ");Serial.println(reading);
 double voltage = reading * 5;voltage/=1024; 
 
 double temperatureC = (voltage - 0.1675) * 100 ;
 
 lcd.setCursor(0,0);lcd.print(temperatureC); lcd.setCursor(6,0);lcd.print(" degrees C");
 
 double temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 lcd.setCursor(0,1);lcd.print(temperatureF); lcd.setCursor(6,1);lcd.print(" degrees F");
 
 delay(1000);
}
