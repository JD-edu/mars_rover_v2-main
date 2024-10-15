#include<Servo.h>

Servo sv1, sv2;
int angle1 = 90, angle2 = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sv1.attach(2);
  sv2.attach(3);
  Serial.println("Mars rover mini start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  int pan = analogRead(0);
  int tilt = analogRead(1);

  if(pan > 700){
    angle1 += 10;
    if(angle1 > 170){
      angle1 = 170;
    }
  }else if(pan < 200){
    angle1 -= 10;
    if(angle1 < 10){
      angle1 = 10;
    }
  }
  sv1.write(angle1);

  if(tilt > 700){
    angle2 += 10;
    if(angle2 > 170){
      angle2 = 170;
    }
  }else if(tilt < 200){
    angle2 -= 10;
    if(angle2 < 10){
      angle2 = 10;
    }
  }
  sv2.write(angle2);
  
  Serial.print(pan);
  Serial.print(" ");
  Serial.println(tilt);
  
  delay(200);
}
