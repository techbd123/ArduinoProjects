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

void ShowNumber(int n)
{
  if(n<10) lcd.print("0");
  lcd.print(n);
}

void ShowClock(struct Clock time)
{
  lcd.setCursor(0,0);
  ShowNumber(time.h);
  lcd.print(":");ShowNumber(time.m);
  lcd.print(":");ShowNumber(time.s);
}

void ShowPrompt()
{
  lcd.setCursor(0,1);lcd.print("Set Clk-1 Alrm-2");
}

char str[MAX];


void SetClock()
{
  lcd.clear();ShowClock(clk);
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
  lcd.print(":");
  while((k=TakeInput())>'5');lcd.print(k);val=k-'0';
  while((k=TakeInput())>'9');lcd.print(k);val*=10;val+=k-'0';
  clk.s=val;
}

void SetAlarm()
{
  lcd.clear();
  if(alrm.h==-1) ShowClock(clk);else ShowClock(alrm);
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
  lcd.print(":");
  while((k=TakeInput())>'5');lcd.print(k);val=k-'0';
  while((k=TakeInput())>'9');lcd.print(k);val*=10;val+=k-'0';
  alrm.s=val;
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
  if(clk.s>59)
  {
    clk.m++;
    clk.s%=60;
  }
  if(clk.m>59)
  {
    clk.h++;
    clk.m%=60;
  }
  clk.h%=24;
}

bool hAlarm()
{
  return clk.h==alrm.h&&clk.m==alrm.m&&clk.s==alrm.s;
}

int delsec=3000;

void loop()
{
  key=NO_KEY;
  if(hAlarm()==true)
  {
    ShowAlarm(delsec);
    clk.s+=delsec/1000;
    Normalize();
  }
  ShowClock(clk);
  ShowPrompt();
  for(int i=1;i<999;i++)
  {
    if(key==NO_KEY) key=keypad.getKey();
    delay(1);
  }
  if(key==NO_KEY) key=keypad.getKey();
  if(key=='1')
  {
    SetClock();
  }
  else if(key=='2')
  {
    long long int prev=millis();
    SetAlarm();
    long long int now=millis();
    now-=prev;now/=1000;
    clk.s+=(int)now;
  }
  else clk.s++;
  Normalize();
}
