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

struct PassWord
{
  char password[MAX];
  int len;bool hset;
  PassWord()
  {
    hset=false;
  }
  void SetPassWord(char *a)
  {
    for(len=0;a[len];len++) password[len]=a[len];
    hset=true;
  }
  bool hMatched(char *a)
  {
    int i;
    for(i=0;i<len;i++)
    {
      if(password[i]==a[i]);else return false;
    }
    if(a[i]) return false;
    return true;
  }
  void RemovePassword()
  {
    hset=false;
  }
}pw;

bool hlocked;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.blink();
  hlocked=false;
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

void ShowPrompt()
{
  lcd.setCursor(0,1);lcd.print("Set New Password");
}


void ShowShortLength(int msec)
{
  lcd.setCursor(0,1);lcd.print("Short Password! ");delay(msec);
  lcd.setCursor(0,1);lcd.print("Length >= 4     ");delay((msec*3)/2);
  lcd.clear();
}

void ShowWrongPassword(int msec)
{
  lcd.setCursor(0,1);lcd.print("Wrong Password! ");delay(msec);lcd.clear();
}

void ShowLocked()
{
  lcd.setCursor(0,1);lcd.print("Locked!         ");
}

void ShowUnlocked()
{
  lcd.setCursor(0,1);lcd.print("UnLocked!       ");
}

void ShowLockedOld(int msec)
{
  lcd.setCursor(0,1);lcd.print("Locked! OLD PSWD");delay(msec);lcd.clear();
}

char str[MAX];

void loop()
{
  int len;
  lcd.clear();
  if(hlocked==true) ShowLocked();else ShowPrompt(); 
  len=TakeString(str);
  if(len<4)
  {
    if(!len&&pw.len&&hlocked==false)
    {
      pw.hset=true;
      hlocked=true;
      ShowLockedOld(1000);
    }
    else ShowShortLength(1000);
  }
  else
  {
      if(pw.hset==false)
      {
        pw.SetPassWord(str);
        hlocked=true;
      }
      else
      {
        if(pw.hMatched(str)==true)
        {
          hlocked=false;
          pw.RemovePassword();
          ShowUnlocked();delay(1000);
        }
        else ShowWrongPassword(1000);
      }
  }
}
