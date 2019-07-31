 // PHOENIXFrameUltrasonicMatrix

#include "UltraMat.h"

UltraMat mat;
String lastStr;
#define NUMMAX 6

void setup() 
{
  Serial.begin(9600);
  mat.Init(0, P_2, P_3);
  mat.Init(1, P_4, P_5);
  mat.Init(2, P_6, P_7);
  mat.Init(3, P_8, P_9);
  mat.Init(4, P_10, P_11);
  mat.Init(5, P_12, P_13);

  pinMode(A0, OUTPUT);
}

//----------------------------------------------------------------------------
String I2Str(int val)
{
  char str[25];
  itoa(val, str, 10); // 10 is decimal
  return str;
}

//String recvStr;
void loop() 
{
  
  mat.Tick();
  int dist[NUMMAX];
  dist[0] = mat.GetDistance(0);
  dist[1] = mat.GetDistance(1);
  dist[2] = mat.GetDistance(2);
  dist[3] = mat.GetDistance(3);
  dist[4] = mat.GetDistance(4);
  dist[5] = mat.GetDistance(5);

  bool isHasNear = false;

  lastStr="";
  for (int i=0; i<NUMMAX; i++)
  {
    float dst = dist[i];
    lastStr += dst;
    if (i != NUMMAX-1)
    {
      lastStr += " ";
    }
    if (dst < 40)
    {
      isHasNear = true;
    }
  }
  
  Serial.println(lastStr);

  if (isHasNear)
  {
     digitalWrite(A0, HIGH);
  }
  else
  {
    digitalWrite(A0, LOW);
   }
}
