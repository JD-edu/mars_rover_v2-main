#include <U8x8lib.h>
#include "BluetoothSerial.h"
#include <SPI.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
//BluetoothSerial SerialBT;

#define M1_B    14
#define M1_A    12
#define M2_B    13
#define M2_A    25
#define M3_B    15
#define M3_A    2
#define M4_B    18
#define M4_A    19
#define M5_B    17
#define M5_A    5
#define M6_B    4
#define M6_A    16

#define s1center 90 
#define s3center 90
#define s4center 90
#define s6center 90

#define SERVO1  26
#define SERVO2  27
#define SERVO3  32
#define SERVO4  33

Servo myservo1, myservo2, myservo3, myservo4;

BluetoothSerial SerialBT;

#define WIFI_SSID   "GBSA0001"
#define WIFI_PASSWD "GBSA0001"

String macAddress = "";
String ipAddress = "";

// Moth server setting 
const char* websockets_server_host = "cobot.center";
const uint16_t websockets_server_port = 8286;
using namespace websockets;
WebsocketsClient client;

void go_forward(){
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);
    digitalWrite(M3_A, HIGH);
    digitalWrite(M3_B, LOW);
    digitalWrite(M4_A, LOW);
    digitalWrite(M4_B, HIGH);
    digitalWrite(M5_A, LOW);
    digitalWrite(M5_B, HIGH);
    digitalWrite(M6_A, LOW);
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

void turn_left(){
    myservo1.write(60);
    myservo2.write(60);
    myservo3.write(120);
    myservo4.write(120);
}

void turn_right(){
    myservo1.write(120);
    myservo2.write(120);
    myservo3.write(60);
    myservo4.write(60);
}

void servo_center(){
    myservo1.write(90);
    myservo2.write(90);
    myservo3.write(90);
    myservo4.write(90);
}

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    /*
    if(message.data().toInt() == 0){
        Serial.println("stop");
        stop();
    }else if(message.data().toInt() == 1){
        Serial.println("go forward");
        go_forward();
    }else if(message.data().toInt() == 2){
        Serial.println("turn left");
        turn_left();
    }else if(message.data().toInt() == 3){
        Serial.println("turn right");
        turn_right();
    }else if(message.data().toInt() == 4){
        Serial.println("go forward");
        go_forward();
    }*/
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed... Rebooting");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setupNetwork()
{
    macAddress = "mars-rover";
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    ipAddress = WiFi.localIP().toString();
    macAddress += WiFi.macAddress().substring(0, 5);      
    
    // Setup Callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect Moth server 
    while(!client.connect(websockets_server_host, websockets_server_port, "pang/ws/pub?channel=c40hp6epjh65aeq6ne50&source=base&track=<label>&mode=single")) {
        delay(500);
        Serial.print(".");
    }

    delay(500);

    Serial.println("");
    Serial.println("Websocket Connected!");
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
    if(event == ESP_SPP_SRV_OPEN_EVT){
        Serial.println("Client Connected");
    //}else if(event == ESP_SPP_SRV_STOP_EVT){
    //    Serial.println("Client Stopped");
    }else if(event == ESP_SPP_CLOSE_EVT){
        Serial.println("Client closed");
    }
}

void setup() {
   
   
    Serial.begin(115200);
    //SerialBT.begin("JEDEW monitor"); //Bluetooth device name
    Serial.print("Starting JEDEW monitor...");

    //u8x8.begin();
    //u8x8.setPowerSave(0);
    //u8x8.setFont(u8x8_font_chroma48medium8_r);
    //u8x8.drawString(0,0,"JEDEW");
    //u8x8.drawString(10,0,"Monitor");
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

    //setupNetwork();
    servo_center();

    SerialBT.register_callback(callback);
    SerialBT.begin("Rover4"); //Bluetooth device name
}



void loop() {
    char inCommand;

    String inString = "";
    if(SerialBT.available() >0){
        if(SerialBT.available()){
            inString = SerialBT.readStringUntil('\n');
            Serial.println(inString);
            inCommand = inString[0];
        }    
    }

    if(inCommand == 'f'){
        go_forward();
    }else if(inCommand == 'b'){
        go_backward();
    }else if(inCommand == 'l'){
        turn_left();
    }else if(inCommand == 'r'){
        turn_right();
    }else if(inCommand == 's'){
        stop();
    }else if(inCommand == 'c'){
        servo_center();
    }

    if(Serial.available() > 0){
        char c = Serial.read();
        if(c == 'f'){
            go_forward();
        }else if(c == 'b'){
            go_backward();
        }else if(c == 'l'){
            turn_left();
        }else if(c == 'r'){
            turn_right();
        }else if(c == 's'){
            stop();
        }else if(c == 'c'){
            servo_center();
        }
    }
    client.poll();

}
