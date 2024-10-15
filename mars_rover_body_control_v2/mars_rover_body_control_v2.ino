#include <U8x8lib.h>
#include "BluetoothSerial.h"
#include <SPI.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
//BluetoothSerial SerialBT;

#define M1_B    26
#define M1_A    27
#define M2_B    12
#define M2_A    14
#define M3_B    15
#define M3_A    2
#define M4_B    4
#define M4_A    16
#define M5_B    5
#define M5_A    18
#define M6_B    21
#define M6_A    19

#define s1center 90 
#define s3center 90
#define s4center 90
#define s6center 90

#define SERVO1  25
#define SERVO2  13
#define SERVO3  17
#define SERVO4  23
#define SERVO5  32
#define SERVO6  33

Servo myservo1, myservo2, myservo3, myservo4, myservo5, myservo6;

#define WIFI_SSID  "SK_WiFiGIGA73BA"
#define WIFI_PASSWD "1609043407"

String macAddress = "";
String ipAddress = "";

// Moth server setting 
//const char* websockets_server_host = "192.168.0.77";
//const uint16_t websockets_server_port = 8276;
const char *websockets_server_host = "cobot.center";
const uint16_t websockets_server_port = 8286;
using namespace websockets;
WebsocketsClient client;

void go_forward(){
    digitalWrite(M1_A, HIGH);
    analogWrite(M1_B, 127);
    digitalWrite(M2_A, HIGH);
    analogWrite(M2_B, 127);
    digitalWrite(M3_A, HIGH);
    analogWrite(M3_B, 127);
    analogWrite(M4_A, 127);
    digitalWrite(M4_B, HIGH);
    analogWrite(M5_A, 127);
    digitalWrite(M5_B, HIGH);
    analogWrite(M6_A, 127);
    digitalWrite(M6_B, HIGH);
}

void go_backward(){
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);
    digitalWrite(M3_A, LOW);
    digitalWrite(M3_B, HIGH);
    digitalWrite(M4_A, HIGH);
    digitalWrite(M4_B, LOW);
    digitalWrite(M5_A, HIGH);
    digitalWrite(M5_B, LOW);
    digitalWrite(M6_A, HIGH);
    digitalWrite(M6_B, LOW);
}

void stop(){
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);
    digitalWrite(M3_A, LOW);
    digitalWrite(M3_B, LOW);
    digitalWrite(M4_A, LOW);
    digitalWrite(M4_B, LOW);
    digitalWrite(M5_A, LOW);
    digitalWrite(M5_B, LOW);
    digitalWrite(M6_A, LOW);
    digitalWrite(M6_B, LOW);
}

void servo_center(){
    myservo1.write(90);
    myservo2.write(90);
    myservo3.write(90);
    myservo4.write(90);
}

void turn_wheel(int angle){
    myservo1.write(angle);
    myservo2.write(angle);
    myservo3.write(180-angle);
    myservo4.write(180-angle);
}

int camera_pan = 90;
int camera_tilt = 90;
void camera_down(){
    camera_tilt += 5;
    if(camera_tilt > 150){
        camera_tilt = 150;
    }
    myservo5.write(camera_tilt);
}

void camera_tilt_turn(int tilt){
    camera_tilt = tilt;
    myservo5.write(camera_tilt);
}

void camera_pan_turn(int pan){
    camera_pan = pan;
    myservo6.write(camera_pan);
}


void camera_up(){
    camera_tilt -= 5;
    if(camera_tilt < 60){
        camera_tilt = 60;
    }
    myservo5.write(camera_tilt);
}

void camera_left(){
    camera_pan += 5;
    if(camera_pan > 150){
        camera_pan = 150;
    }
    myservo6.write(camera_pan);
}

void camera_right(){
    camera_pan -= 5;
    if(camera_pan < 60){
        camera_pan = 60;
    }
    myservo6.write(camera_pan);
}

void camera_center(){
    camera_pan = 90;
    camera_tilt = 90;
    myservo6.write(camera_pan);
    myservo5.write(camera_tilt);
}



void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    if (message.isText())
    {
        const char *command = message.c_str();
        Serial.println(command);
        if (strcmp(command, "AU") == 0){
            stop();
            delay(100);
            go_forward();
        }
        if (strcmp(command, "AD") == 0){
            stop();
            delay(100);
            go_backward();
        }
        if (strncmp(command, "AL", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('L');
            int second = cmd.indexOf('Z');
            String angleStr = cmd.substring(first+1, second);
            int angle = angleStr.toInt();
            turn_wheel(angle);
        }
        if (strncmp(command, "AR", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('R');
            int second = cmd.indexOf('Z');
            String angleStr = cmd.substring(first+1, second);
            int angle = angleStr.toInt();
            turn_wheel(angle);
        }
        if (strcmp(command, "STOP") == 0){
            stop();
        }
        if (strcmp(command, "CENTER") == 0){
            servo_center();
        }
        if (strncmp(command, "CU", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('U');
            int second = cmd.indexOf('Z');
            String tiltStr = cmd.substring(first+1, second);
            int tilt = tiltStr.toInt();
            camera_tilt_turn(tilt);
        }
        if (strncmp(command, "CD", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('D');
            int second = cmd.indexOf('Z');
            String tiltStr = cmd.substring(first+1, second);
            int tilt = tiltStr.toInt();
            camera_tilt_turn(tilt);
        }
        if (strncmp(command, "CR", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('R');
            int second = cmd.indexOf('Z');
            String panStr = cmd.substring(first+1, second);
            int pan = panStr.toInt();
            camera_pan_turn(pan);
        }
        if (strncmp(command, "CL", 2) == 0){
            String cmd = command;
            int first = cmd.indexOf('L');
            int second = cmd.indexOf('Z');
            String panStr = cmd.substring(first+1, second);
            int pan = panStr.toInt();
            camera_pan_turn(pan);
        }
        //if (strcmp(command, "C_CENTER") == 0){
        //    camera_center();
        //}
    }
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        digitalWrite(22, HIGH);
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        digitalWrite(22, LOW);
        delay(100);
        Serial.println("Connnection Closed... Rebooting");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setup() {
   
   
    Serial.begin(115200);
    //SerialBT.begin("JD-edu mars rover Start..."); //Bluetooth device name
    Serial.print("JD-edu mars rover Start...");

    //u8x8.begin();
    //u8x8.setPowerSave(0);
    //u8x8.setFont(u8x8_font_chroma48medium8_r);
    //u8x8.drawString(0,0,"JEDEW");
    //u8x8.drawString(10,0,"Monitor");
    pinMode(22, OUTPUT);
    pinMode(M1_A, OUTPUT);
    pinMode(M1_B, OUTPUT);
    pinMode(M2_A, OUTPUT);
    pinMode(M2_B, OUTPUT);
    pinMode(M3_A, OUTPUT);
    pinMode(M3_B, OUTPUT);
    pinMode(M4_A, OUTPUT);
    pinMode(M4_B, OUTPUT);
    pinMode(M5_A, OUTPUT);
    pinMode(M5_B, OUTPUT);
    pinMode(M6_A, OUTPUT);
    pinMode(M6_B, OUTPUT);

    digitalWrite(23, LOW);

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    myservo1.setPeriodHertz(50);    // standard 50 hz servo
    myservo1.attach(SERVO1, 500, 2400);
    myservo2.setPeriodHertz(50); 
    myservo2.attach(SERVO2, 500, 2400);
    myservo3.setPeriodHertz(50); 
    myservo3.attach(SERVO3, 500, 2400);
    myservo4.setPeriodHertz(50); 
    myservo4.attach(SERVO4, 500, 2400);
    myservo6.setPeriodHertz(50); 
    myservo6.attach(SERVO5, 500, 2400);
    myservo5.setPeriodHertz(50); 
    myservo5.attach(SERVO6, 500, 2400);

    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi Connected.");

    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    // c3rl3c86n88jq9lrl3gg MarsRover-1
    while (!client.connect(websockets_server_host, websockets_server_port, "/pang/ws/pub?channel=c3rl3c86n88jq9lrl3gg&track=colink&mode=bundle"))
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Websocket Connected.");
    servo_center();
    camera_center();
}



void loop() {
    if(Serial.available() > 0){
        char c = Serial.read();
        if(c == 'f'){
            go_forward();
        }else if(c == 'b'){
            go_backward();
        }else if(c == 'l'){
            //turn_left();
        }else if(c == 'r'){
            //turn_right();
        }else if(c == 'p'){
            stop();
        }else if(c == 'c'){
            servo_center();
        }else if(c == 'a'){
            camera_left();
        }else if(c == 'd'){
            camera_right();
        }else if(c == 'w'){
            camera_up();
        }else if(c == 's'){
            camera_down();
        }else if(c == 'x'){
            camera_center();
        }
    }
    client.poll();
    client.send("123");
}
