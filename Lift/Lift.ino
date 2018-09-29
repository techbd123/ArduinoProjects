#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#define Maximum(a,b) (a)>(b)?(a):(b)
#define Minimum(a,b) (a)<(b)?(a):(b)
#define MAX 15
#define MAX_FLOOR 5

using namespace std;

LiquidCrystal lcd(14,15,16,17,18,19);

const byte ROW=4;
const byte COL=4;
byte rowPin[ROW]={
  9,8,7,6};
byte colPin[COL]={
  5,4,3,2};

char keys[ROW][COL]={
  {
    '1','2','3','A'  }
  ,
  {
    '4','5','6','B'  }
  ,
  {
    '7','8','9','C'  }
  ,
  {
    '*','0','#','D'  }
  ,
};


Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,ROW,COL);

struct Floor
{
  bool up,down;
  bool hAppointed()
  {
    return up|down;
  }
  void Initialize()
  {
    up=down=false;
  }
}
floors[MAX_FLOOR];

struct Lift
{
  int direction;
  int currentFloor;
  int destination;
  void Initialize()
  {
    direction=currentFloor=destination=0;
  }
}
lift;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lift.Initialize();
  for(int i=0;i<MAX_FLOOR;i++) floors[i].Initialize();
}


char TakeInput()
{
  char key;
  while((key=keypad.getKey())==NO_KEY);
  return key;
}

void ShowFloor(int c)
{
  lcd.setCursor(0,1);
  lcd.print("Floor ");
  if(c=='0'||!c) lcd.print("G");
  else lcd.print(c);
}

void ShowUp()
{
  lcd.setCursor(0,0);
  lcd.print("              UP");
  lcd.setCursor(12,1);
  lcd.print("    ");
}

void ShowDown()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(12,1);
  lcd.print("DOWN");
}

void ChangeFloorState(char key)
{
  Serial.print(key);
  for(int i=1;i<10;i++)
  {
    if(i+'0'==key)
    {
      if(i%2) floors[i/2].up=true;
      else floors[i/2].down=true;
    }
  }
}

void ShowOpen()
{
  lcd.setCursor(0,0);
  lcd.print("Open            ");
  lcd.setCursor(12,1);
  lcd.print("    ");
}

char key;

void Delay(int millisec)
{
  for(int i=0;i<millisec;i++)
  {
    if(key==NO_KEY)
    {
      key=keypad.getKey();
    }
    if(key!=NO_KEY) ChangeFloorState(key),key=NO_KEY;
    delay(1);
  }
}

int FindDestination()
{
  int dest=lift.destination,dest_min[2]={MAX_FLOOR,MAX_FLOOR},dest_max[2]={-1,-1};
  for(int i=0;i<MAX_FLOOR;i++)
  {
    if(floors[i].hAppointed())
    {
      if(lift.direction==1)
      {
        if(floors[i].up) dest_min[0]=Minimum(dest_min[0],i);
        if(floors[i].down) dest_max[0]=Maximum(dest_max[0],i);
      }
      if(lift.direction==-1)
      {
        if(floors[i].down) dest_max[1]=Maximum(dest_max[1],i);
        if(floors[i].up) dest_min[1]=Minimum(dest_min[1],i);
      }
      if(!lift.direction) dest=i;
    }
  }
  if(dest_min[0]!=MAX_FLOOR)  return dest_min[0];
  if(dest_max[0]!=-1)  return Maximum(dest_max[0],dest);
  if(dest_max[1]!=-1)  return dest_max[1];
  if(dest_min[1]!=MAX_FLOOR)  return Minimum(dest_min[1],dest);
  return dest;
}

void loop()
{
  ShowFloor(lift.currentFloor);
  Delay(500);
  if(lift.direction!=0) lift.destination=FindDestination();
  if(lift.currentFloor==lift.destination)
  {
    ShowOpen();
    Delay(1000);
    if(lift.direction==0)
    {
      if(floors[lift.currentFloor].up)
      {
        lift.destination=MAX_FLOOR-1;
        floors[lift.currentFloor].up=false;
      }
      else if(floors[lift.currentFloor].down)
      {
        lift.destination=0;
        floors[lift.currentFloor].down=false;
      }
      else lift.destination=FindDestination();
    }
    else if(lift.direction==1)
    {
      if(floors[lift.currentFloor].up)
      {
        lift.destination=MAX_FLOOR-1;
        floors[lift.currentFloor].up=false;
      }
    }
    else
    {
      if(floors[lift.currentFloor].down)
      {
        lift.destination=0;
        floors[lift.currentFloor].down=false;
      }
    }
    if(lift.direction!=0) lift.destination=FindDestination();
    lift.direction=0;
  }
  else
  {
    if(lift.destination==-1||lift.destination==MAX_FLOOR)
    {
      lift.direction=0;
    }
    else if(lift.currentFloor<lift.destination)
    {
      ShowUp();
      lift.direction=1;
    }
    else if(lift.currentFloor>lift.destination)
    {
      ShowDown();
      lift.direction=-1;
    }
    lift.currentFloor+=lift.direction;
  }
}
