//       FRONT
//  M1S1       M6S6
//    \         /
//     \       /
//  M2-----------M5
//     /       \
//    /         \
//  M3S3       M4S4
//        REAR

#include <Servo.h>

#define s1center 90 // posiciones de centrado de los servos
#define s3center 90
#define s4center 90
#define s6center 90

#define s1giro 135 // posiciones de giro
#define s3giro 45
#define s4giro 150
#define s6giro 35

Servo servo1;
Servo servo3;
Servo servo4;
Servo servo6;

#define D1 3
#define D2 4
#define D3 5
#define D4 6

#define left_1_1 14
#define left_1_2 15
#define right_1_1 16
#define right_1_2 17
#define right_2_1 42
#define right_2_2 43
#define left_2_1 44
#define left_2_2 45
#define left_3_1 18
#define left_3_2 19
#define right_3_1 26
#define right_3_2 27

void go_forward(){
  Serial1.println("go_forward()");
  digitalWrite(left_1_1, HIGH);
  digitalWrite(left_1_2, LOW);
  digitalWrite(right_1_1, HIGH);
  digitalWrite(right_1_2, LOW);
  digitalWrite(left_2_1, HIGH);
  digitalWrite(left_2_2, LOW);
  digitalWrite(right_2_1, HIGH);
  digitalWrite(right_2_2, LOW);
  digitalWrite(left_3_1, HIGH);
  digitalWrite(left_3_2, LOW);
  digitalWrite(right_3_1, HIGH);
  digitalWrite(right_3_2, LOW);
}

void go_backward(){
  Serial1.println("go_backward()");
  digitalWrite(left_1_1, LOW);
  digitalWrite(left_1_2, HIGH);
  digitalWrite(right_1_1, LOW);
  digitalWrite(right_1_2, HIGH);
  digitalWrite(left_2_1, LOW);
  digitalWrite(left_2_2, HIGH);
  digitalWrite(right_2_1, LOW);
  digitalWrite(right_2_2, HIGH);
  digitalWrite(left_3_1, LOW);
  digitalWrite(left_3_2, HIGH);
  digitalWrite(right_3_1, LOW);
  digitalWrite(right_3_2, HIGH);
}

void turn_left(){
  Serial1.println("turn_left()");
}

void turn_right(){
  Serial1.println("turn_right()");
}

void move_steering(int angle){
  Serial1.println("move_steering()");
}

void stop(){
    digitalWrite(left_1_1, LOW);
  digitalWrite(left_1_2, LOW);
  digitalWrite(right_1_1, LOW);
  digitalWrite(right_1_2, LOW);
  digitalWrite(left_2_1, LOW);
  digitalWrite(left_2_2, LOW);
  digitalWrite(right_2_1, LOW);
  digitalWrite(right_2_2, LOW);
  digitalWrite(left_3_1, LOW);
  digitalWrite(left_3_2, LOW);
  digitalWrite(right_3_1, LOW);
  digitalWrite(right_3_2, LOW);
}


void setup() {

  Serial.begin(115200);
  Serial.println("mars rover start");

  Serial1.begin(115200);
  Serial1.println("mars rover start");
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT); 
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  
  servo1.attach(10); //pines para servos
  servo3.attach(11);
  servo4.attach(8);
  servo6.attach(9);
  
  servo1.write(s1center); //arranque en posicion central
  servo3.write(s3center);
  servo4.write(s4center);
  servo6.write(s6center);
  
}

void loop() {
  if(Serial1.available()>0){
    char c = Serial1.read();
    Serial.print(c);
    //if(inString[0] == 'f'){
    //  go_forward();
    //}else if(inString[0] == 'b'){
    //  go_backward();
    //}
  }

/*
  delay(3000);
  
  servo1.write(120); 
  servo3.write(120);
  servo4.write(120);
  servo6.write(120);

  delay(2000);
  
  servo1.write(90); 
  servo3.write(90);
  servo4.write(90);
  servo6.write(90);

  delay(2000);

  servo1.write(60); 
  servo3.write(60);
  servo4.write(60);
  servo6.write(60);

  delay(2000);
  
  servo1.write(90); 
  servo3.write(90);
  servo4.write(90);
  servo6.write(90);
*/
 
}
