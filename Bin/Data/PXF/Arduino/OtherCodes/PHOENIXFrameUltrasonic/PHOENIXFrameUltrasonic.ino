int pinL=4;
int pinR=7;
int pinLSpeed = 5;
int pinRSpeed = 6;

void vehicleInitMotoBoard()
{
  pinL = 12;
  pinR = 13;
  pinLSpeed = 10;
  pinRSpeed = 11;

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

int PinTrig = 2;
int PinEcho = 3;
float Dist;
void distInit(int pinTrig, int pinEcho)
{
    PinTrig = pinTrig;
    PinEcho = pinEcho;
    pinMode(PinTrig, OUTPUT);   // 定义超声波输出脚  
    pinMode(PinEcho, INPUT);    // 定义超声波输入脚
}
void distTest()   // 量出前方距离 
{
  digitalWrite(PinTrig, LOW); //低高低电平发一个短时间脉冲去TrigPin 
  delayMicroseconds(2); 
  digitalWrite(PinTrig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(PinTrig, LOW); 
  
  Dist = pulseIn(PinEcho, HIGH) / 58.0; //将回波时间换算成cm 
  Dist = (int(Dist * 100.0)) / 100.0; //保留两位小数  
}

void setup()
{
  vehicleInitMotoBoard();

  distInit(2, 3);
  
  Serial.begin(9600);
}

int turnSpeed = 255;
int goSpeed = 255;

void loop()
{   
  distTest();
  if (Dist> 0 && Dist < 15)
  {
    turnL(turnSpeed);
  }
  else
  {
    goF(goSpeed);
  }  
}
