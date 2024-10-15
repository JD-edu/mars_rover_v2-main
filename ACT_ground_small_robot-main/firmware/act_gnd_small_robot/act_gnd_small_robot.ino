#include "Adafruit_VL53L0X.h"
#include "MPU9250.h"
#include "eeprom_utils.h"
#include "Wire.h"
//#include <U8x8lib.h>
#include "BluetoothSerial.h"
#include <SPI.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
MPU9250 mpu;

unsigned long timer = 0;

#define M1_B    26
#define M1_A    27
#define M2_B    12
#define M2_A    14
#define M3_B    15
#define M3_A    2
#define M4_B    4
#define M4_A    16

#define CONNECTED   23

#define WIFI_SSID  "jdedu9807"
#define WIFI_PASSWD "jdedu9807"

String macAddress = "";
String ipAddress = "";

//const char *websockets_server_host = "cobot.center";
//const uint16_t websockets_server_port = 8286;
const char *websockets_server_host = "192.168.0.77";
const uint16_t websockets_server_port = 8276;
using namespace websockets;
WebsocketsClient client;

// set the pins to shutdown
#define SHT_LOX1 32
#define SHT_LOX2 33
#define SHT_LOX3 25
#define SHT_LOX4 13

#define LOX1_ADDRESS 0x40
#define LOX2_ADDRESS 0x41
#define LOX3_ADDRESS 0x42
#define LOX4_ADDRESS 0x43

// Sensor variables
int sensor1,sensor2, sensor3, sensor4;
int current_angle, start_angle, end_angle;

int beat_count = 0;
int turnState = 0;

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;


bool isTurnOver = true;
void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    if (message.isText())
    {
        const char *command = message.c_str();
        //Serial.println(command);
        if (strcmp(command, "AU") == 0){
            go_forward();
        }
        if (strcmp(command, "AD") == 0){
            go_backward();
        }
        if (strncmp(command, "AL", 2) == 0){
            turn_left();
        }
        if (strncmp(command, "AR", 2) == 0){
            turn_right();
        }
        if (strcmp(command, "STOP") == 0){
            stop();
        }
    }
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        digitalWrite(CONNECTED, HIGH);
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        digitalWrite(CONNECTED, LOW);
        delay(100);
        Serial.println("Connnection Closed... Rebooting");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void read_dual_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

  // sensor one reading
  if(measure1.RangeStatus != 4) {     // if not out of range
    sensor1 = measure1.RangeMilliMeter;    

  } else {
    //Serial.print("Out of range");
  }
  // sensor two reading
  if(measure2.RangeStatus != 4) {
    sensor2 = measure2.RangeMilliMeter;
  } else {
    //Serial.print("Out of range");
  }

  // sensor three reading
  if(measure3.RangeStatus != 4) {
    sensor3 = measure3.RangeMilliMeter;
  } else {
  //  Serial.print("Out of range");
  }

  // sensor four reading
  if(measure4.RangeStatus != 4) {
    sensor4 = measure4.RangeMilliMeter;
  } else {
  //  Serial.print("Out of range");
  }
}

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    digitalWrite(CONNECTED, LOW);
    while(1){
      if(!lox1.begin(LOX1_ADDRESS)){
        break;
      }
    }
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1){
     
      if(!lox2.begin(LOX2_ADDRESS)){
        break;
      }
    
    }
  }

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1){
       if(!lox3.begin(LOX3_ADDRESS)){
        break;
      }
    };
  }

  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);
  //initing LOX4
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1){
      if(!lox3.begin(LOX3_ADDRESS)){
        break;
      }
    };
  }
}

void go_forward(){
  Serial.println("forward");
   digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);

    digitalWrite(M3_A, HIGH);
    digitalWrite(M3_B, LOW);
    digitalWrite(M4_A, HIGH);
    digitalWrite(M4_B, LOW);
   
}

void go_backward(){
  Serial.println("backward");
   digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);

    digitalWrite(M3_A, LOW);
    digitalWrite(M3_B, HIGH);
    digitalWrite(M4_A, LOW);
    digitalWrite(M4_B, HIGH);
}

void turn_right(){
  Serial.println("right");
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);

    digitalWrite(M3_A, LOW);
    digitalWrite(M3_B, HIGH);
    digitalWrite(M4_A, LOW);
    digitalWrite(M4_B, HIGH);
}

void turn_left(){
  Serial.println("left");
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);

    digitalWrite(M3_A, HIGH);
    digitalWrite(M3_B, LOW);
    digitalWrite(M4_A, HIGH);
    digitalWrite(M4_B, LOW);
}

void stop(){
  Serial.println("stop");
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);
    
    digitalWrite(M3_A, LOW);
    digitalWrite(M3_B, LOW);
    digitalWrite(M4_A, LOW);
    digitalWrite(M4_B, LOW);
    delay(200);
}


void setup() {
  Serial.begin(115200);
  Serial.print("ACT ground robot Start...");

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);



  pinMode(M1_A, OUTPUT);
  pinMode(M1_B, OUTPUT);
  pinMode(M2_A, OUTPUT);
  pinMode(M2_B, OUTPUT);
  pinMode(M3_A, OUTPUT);
  pinMode(M3_B, OUTPUT);
  pinMode(M4_A, OUTPUT);
  pinMode(M4_B, OUTPUT);

  pinMode(CONNECTED, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

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
  // c3rl3c86n88jq9lrl3gg LegoMars-1
  // c3rl3f86n88jq9lrl3hg LegoMars-2
  // c3rl3jg6n88jq9lrl3ig LegoMars-3
  // c3rl3l06n88jq9lrl3jg LegoMars-4
  // c3rl3to6n88jq9lrl3lg LegoMars-5
  // c3rl4006n88jq9lrl3mg LegoMars-6
  // c3rl4286n88jq9lrl3ng LegoMars-7
  // c3rl43o6n88jq9lrl3og LegoMars-8
  // c3rl4586n88jq9lrl3pg LegoMars-9
  // cakjdd4k058s72qr0prg MarsCamera-1
  // cakjdgsk058s72qr0psg MarsCamera-2
  // cakjdjck058s72qr0ptg MarsCamera-3rsCAmera-4

  while (!client.connect(websockets_server_host, websockets_server_port, "/pang/ws/pub?channel=c3rl3c86n88jq9lrl3gg&track=colink&mode=bundle"))
  {
      delay(500);
      Serial.print(".");
  }
  

  Serial.println("Websocket Connected.");
  Wire.begin();

  delay(500);
  
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  Serial.println("VL53L0X: Both in reset mode...(pins are low)");
  
  
  Serial.println("VL53L0X: Starting...");
  setID();

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }

#if defined(ESP_PLATFORM) || defined(ESP8266)
  EEPROM.begin(0x80);
#endif

  delay(5000);

  if(isCalibrated()){
    // load from eeprom
    loadCalibration();
  }else{
    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);

    // save to eeprom
    saveCalibration();
    loadCalibration();
  }
  delay(500);
}

void loop() {
  
  if(Serial.available() > 0){
    char c = Serial.read();
    Serial.println(c);
        if(c == 'f'){
        go_forward();
    }else if(c == 'b'){
        go_backward();
    }else if(c == 'l'){
        turn_left();
    }else if(c == 'r'){
        turn_right(); 
    }else if(c == 'p'){
        stop();
    }else if(c == 'h'){

    }
  }
    
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 25) {
      print_roll_pitch_yaw();
      read_dual_sensors(); 
      prev_ms = millis();
    }
  } 

  client.poll();
  beat_count++;
  if(beat_count > 5){
    digitalWrite(CONNECTED, !digitalRead(CONNECTED));
    String data = 'a'+String(current_angle)+'b'+String(sensor1)+'c'+String(sensor2)+'d'+String(sensor3)+'e'+String(sensor4)+'f';
    client.send(data);
    beat_count = 0;
    Serial.print(current_angle);
    Serial.print(' ');
    Serial.print(sensor1);
    Serial.print(' ');
    Serial.print(sensor2);
    Serial.print(' ');
    Serial.print(sensor3);
    Serial.print(' ');
    Serial.println(sensor4);
  }
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}




void print_calibration() {
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}
