#include <LiquidCrystal.h>
#define MOD(a,m) (((a)%(m)+(m))%(m))

LiquidCrystal lcd(14,15,16,17,18,19);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
}

char msg[17]="It's Simple";
int c=15;

void loop()
{
  if(c) lcd.setCursor(c,0);else lcd.setCursor(0,0);
  int i;
  for(i=c<0?-c:0;i<msg[i];i++) lcd.print(msg[i]);while(i<16) lcd.print(" "),i++;
  delay(500);
  c--;
  if(c==-16) c=15;
}
