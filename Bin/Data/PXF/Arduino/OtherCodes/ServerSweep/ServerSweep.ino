/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo0;
Servo myservo1; 
Servo myservo2; 
Servo myservo3; 
Servo myservo4; 
Servo myservo5; 

int pos = 0;

void setup()
{
  myservo0.attach(A0);
  myservo1.attach(A1);
  myservo2.attach(A2);
  myservo3.attach(A3);
  myservo4.attach(A4);
  myservo5.attach(A5);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) 
  { 
    myservo0.write(pos);
    myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1)
  { 
    myservo0.write(pos);
    myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
    myservo4.write(pos);
    myservo5.write(pos);
    delay(15);
  }
}
