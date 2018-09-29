#include <Keypad.h>
#define DELAYS 500

const byte ROW=4;
const byte COL=4;
byte rowPin[ROW]={9,8,7,6};
byte colPin[COL]={5,4,3,2};

char keys[ROW][COL]=
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,ROW,COL);

int pin[3]={12,11,10};

char key;
int ms;
bool off;

void setup()
{
  Serial.begin(9600);
  int i;
  for(i=0;i<3;i++) pinMode(pin[i],OUTPUT);
  key=-1;
  ms=DELAYS;
  off=false;
}

void Delay(int milisec)
{
  int i;key=NO_KEY;
  for(i=0;i<milisec;i++)
  {
    if(key==NO_KEY) key=keypad.getKey();
    delay(1);
  }
}

void Input()
{
  while((key=keypad.getKey())==NO_KEY);
}

void loop()
{
  if(key==-1) Input();
  int i;
  for(i=0;i<3;i++)
  {
    if(off==false)
    {
      digitalWrite(pin[i],1);
    }
    if(key>='0'&&key<='9') ms=DELAYS*(key-'0');
    else if(key=='A')
    {
      off=1-off;
      if(off==false) i=-1;
      else digitalWrite(pin[i],0);
    }
    Delay(ms);
    digitalWrite(pin[i],0);
  }
}
