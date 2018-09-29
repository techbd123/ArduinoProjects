
int pin[8]={2,3,4,5,6,7,8,9};
int swtch=10;

bool seven_segment[11][8]=
{
  {true,true,true,false,true,true,true,false},
  {true,false,false,false,false,false,true,false},
  {true,true,false,true,true,true,false,false},
  {true,true,false,true,false,true,true,false},
  {true,false,true,true,false,false,true,false},
  {false,true,true,true,false,true,true,false},
  {false,true,true,true,true,true,true,false},
  {true,true,false,false,false,false,true,false},
  {true,true,true,true,true,true,true,false},
  {true,true,true,true,false,true,true,false},
  {false,false,false,false,false,false,false,true}
};

bool switch_val,prev_val;

int num;

void Toggle()
{
  if(switch_val==true)
  {
    prev_val=1-prev_val;
  }
}

void setup()
{
  Serial.begin(9600);
  int i;
  for(i=0;i<8;i++) pinMode(pin[i],OUTPUT);
  pinMode(swtch,INPUT);
  num=0;
  prev_val=switch_val=false;
}

void Show(int n)
{
  int i;
  for(i=0;i<8;i++) digitalWrite(pin[i],seven_segment[n][i]);
}

void Delay(int milisec)
{
  int i;switch_val=false;
  for(i=0;i<milisec;i+=4)
  {
    if(switch_val==false) switch_val=digitalRead(swtch);
    delay(4);
  }
  Toggle();
}

void loop()
{
  Show(num);
  Delay(1000);
  if(switch_val==prev_val)
  {
    num++;num%=10;
  }
}
