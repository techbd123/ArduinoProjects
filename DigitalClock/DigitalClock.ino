#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#define MAX 15

using namespace std;

LiquidCrystal lcd(14,15,16,17,18,19);

const byte ROW=4;
const byte COL=4;
byte rowPin[ROW]={9,8,7,6};
byte colPin[COL]={5,4,3,2};

char keys[ROW][COL]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};


Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,ROW,COL);

struct Clock
{
  int h,m,s;
}clk,alrm;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.blink();
  clk.h=clk.m=clk.s=0;
  alrm.h=alrm.m=alrm.s=-1;
}


char TakeInput()
{
  char key;
  while((key=keypad.getKey())==NO_KEY);
  return key;
}

int TakeString(char *a)
{
  int i=0;char key;lcd.setCursor(0,0);
  while((key=TakeInput())!='A')
  {
    if(key=='#')
    {
      if(i>0)
      {
        i--;
        lcd.rightToLeft();lcd.print(" ");lcd.leftToRight();lcd.print(" ");
        lcd.rightToLeft();lcd.print(" ");lcd.leftToRight();
      }
      continue;
    }
    lcd.print(key);
    a[i++]=key;
  }
  a[i]=0;
  return i;
}

void ShowNumber(int n)
{
  if(n<10) lcd.print("0");
  lcd.print(n);
}

void ShowClock()
{
  lcd.setCursor(0,0);
  ShowNumber(clk.h);
  lcd.print(":");ShowNumber(clk.m);
  //lcd.print(":");ShowNumber(clk.s);
}

void ShowPrompt()
{
  lcd.setCursor(0,1);lcd.print("Set Clk-1 Alrm-2");
}

char str[MAX];


void SetClock()
{
  lcd.clear();ShowClock();
  lcd.setCursor(0,0);lcd.blink();
  char k;int val;
  while((k=TakeInput())>'2');lcd.print(k);val=k-'0';
  if(k=='2')
  {
    while((k=TakeInput())>'3');
  }
  else
  {
    while((k=TakeInput())>'9');
  }
  lcd.print(k);val*=10;val+=k-'0';
  clk.h=val;
  lcd.print(":");
  while((k=TakeInput())>'5');lcd.print(k);val=k-'0';
  while((k=TakeInput())>'9');lcd.print(k);val*=10;val+=k-'0';
  clk.m=val;
}

void SetAlarm()
{
  lcd.clear();ShowClock();
  lcd.setCursor(0,0);lcd.blink();
  char k;int val;
  while((k=TakeInput())>'2');lcd.print(k);val=k-'0';
  if(k=='2')
  {
    while((k=TakeInput())>'3');
  }
  else
  {
    while((k=TakeInput())>'9');
  }
  lcd.print(k);val*=10;val+=k-'0';
  alrm.h=val;
  lcd.print(":");
  while((k=TakeInput())>'5');lcd.print(k);val=k-'0';
  while((k=TakeInput())>'9');lcd.print(k);val*=10;val+=k-'0';
  alrm.m=val;
}

void ShowAlarm(int milsec)
{
  lcd.clear();
  lcd.print("    ALARM !!!   ");
  delay(milsec);
  lcd.clear();
}

char key;

void Normalize()
{
  if(clk.m>59)
  {
    clk.h++;
    clk.m%=60;
  }
  clk.h%=12;
}

void loop()
{
  key=NO_KEY;int delsec=2000;
  if(clk.h==alrm.h&&clk.m==alrm.m)
  {
    ShowAlarm(delsec);
    clk.m+=delsec/1000;
    Normalize();
  }
  ShowClock();
  ShowPrompt();
  for(int i=1;i<1000;i++)
  {
    if(key==NO_KEY) key=keypad.getKey();
    delay(1);
  }
  clk.m++;Normalize();
  if(key==NO_KEY) key=keypad.getKey();
  if(key=='1')
  {
    SetClock();
  }
  else if(key=='2')
  {
    int prev=millis();
    SetAlarm();
    int now=millis();
    now-=prev;now/=1000;
    clk.m+=now;
    Normalize();
  }
}
