int pinL=4;
int pinR=7;
int pinLSpeed = 5;
int pinRSpeed = 6;

int leftLineFollow = 12;
int rightLineFollow = A1;

void vehicleInitMotoBoard4567()
{
  pinL = 4;
  pinR = 7;
  pinLSpeed = 5;
  pinRSpeed = 6;

  pinMode(pinL, OUTPUT);  
  pinMode(pinR, OUTPUT);
  pinMode(pinLSpeed, OUTPUT);
  pinMode(pinRSpeed, OUTPUT);
}

void turnL(int val)
{
   digitalWrite(pinL,LOW);
   analogWrite(pinLSpeed, val);
   digitalWrite(pinR,HIGH);
   analogWrite(pinRSpeed, val); 
}

void turnR(int val)
{
   digitalWrite(pinL,HIGH);
   analogWrite(pinLSpeed, val);
   digitalWrite(pinR,LOW);
   analogWrite(pinRSpeed, val); 
}

void goF(int val)
{
   digitalWrite(pinL,HIGH);
   analogWrite(pinLSpeed, val);
   digitalWrite(pinR,HIGH);
   analogWrite(pinRSpeed, val); 
}

void goB(int val)
{
   digitalWrite(pinL,LOW);
   analogWrite(pinLSpeed, val);
   digitalWrite(pinR,LOW);
   analogWrite(pinRSpeed, val); 
}

void setup()
{
  vehicleInitMotoBoard4567();
  pinMode(leftLineFollow, INPUT);
  pinMode(rightLineFollow, INPUT);
  Serial.begin(9600);
}

void loop()
{ 
  int goVal = 100;
  int turnVal = 80;
  int num1 = 0;
  int num2 = 0;
  num1=digitalRead(leftLineFollow);    
  num2=digitalRead(rightLineFollow);
  Serial.print("num1:");
  Serial.println(num1);
  Serial.print("num2:");
  Serial.println(num2);

  if(num1==1) 
  {
   turnL(turnVal);   
   delayMicroseconds(2);
  }
  else if(num2==1) 
  {
     turnR(turnVal);
     delayMicroseconds(2);
  }
  else                     //其他状态小车直走
  {
   goF(goVal);
   delay(2);
  }
}
