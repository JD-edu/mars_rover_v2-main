#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <SPI.h>
#include <ESP32Servo.h>

#define M1_B 14
#define M1_A 12
#define M2_B 13
#define M2_A 25
#define M3_B 15
#define M3_A 2
#define M4_B 18
#define M4_A 19
#define M5_B 17
#define M5_A 5
#define M6_B 4
#define M6_A 16

#define s1center 90
#define s3center 90
#define s4center 90
#define s6center 90

#define SERVO1 26
#define SERVO2 27
#define SERVO3 32
#define SERVO4 33

Servo myservo1, myservo2, myservo3, myservo4;

#define WIFI_SSID "teamgrit-demo-2G" // Set your own WiFi SSID
#define WIFI_PASSWD "teamgritdemo8266" // Set your own WiFi PASSWORD

const char *websockets_server_host = "cobot.center";
const uint16_t websockets_server_port = 8286;
using namespace websockets;
WebsocketsClient client;

void go_forward()
{
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

void go_backward()
{
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
 
void stop()
{
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

void turn_left()
{
  myservo1.write(60);
  myservo2.write(60);
  myservo3.write(120);
  myservo4.write(120);
}

void turn_right()
{
  myservo1.write(120);
  myservo2.write(120);
  myservo3.write(60);
  myservo4.write(60);
}

void servo_center()
{
  myservo1.write(90);
  myservo2.write(90);
  myservo3.write(90);
  myservo4.write(90);
}

void onMessageCallback(WebsocketsMessage message)
{
  Serial.print("Got Message: ");

  if (message.isText())
  {
    const char *command = message.c_str();
    Serial.println(command);
    if (strcmp(command, "GF") == 0)
    {
      go_forward();
    }

    if (strcmp(command, "GB") == 0)
    {
      go_backward();
    }

    if (strcmp(command, "TL") == 0)
    {
      turn_left();
    }

    if (strcmp(command, "TR") == 0)
    {
      turn_right();
    }

    if (strcmp(command, "STOP") == 0)
    {
      stop();
    }

    if (strcmp(command, "CENTER") == 0)
    {
      servo_center();
    }
  }
}

void onEventsCallback(WebsocketsEvent event, String data)
{
  if (event == WebsocketsEvent::ConnectionOpened)
  {
    Serial.println("Connnection Opened");
  }
  else if (event == WebsocketsEvent::ConnectionClosed)
  {
    Serial.println("Connnection Closed... Rebooting");
    ESP.restart();
    servo_center();
    stop();
  }
  else if (event == WebsocketsEvent::GotPing)
  {
    Serial.println("Got a Ping!");
  }
  else if (event == WebsocketsEvent::GotPong)
  {
    Serial.println("Got a Pong!");
  }
}

// void setupNetwork()
// {
//   WiFi.begin(WIFI_SSID, WIFI_PASSWD);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi Connected.");

//   while (!client.connect(websockets_server_host, websockets_server_port, "pang/ws/pub?channel=instant&name=mars_test&track=colink"))
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   delay(500);
//   Serial.println("");
//   Serial.println("Websocket Connected.");
// }

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

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

  myservo1.setPeriodHertz(50);
  myservo1.attach(SERVO1, 500, 2400);
  myservo2.setPeriodHertz(50);
  myservo2.attach(SERVO2, 500, 2400);
  myservo3.setPeriodHertz(50);
  myservo3.attach(SERVO3, 500, 2400);
  myservo4.setPeriodHertz(50);
  myservo4.attach(SERVO4, 500, 2400);

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

  while (!client.connect(websockets_server_host, websockets_server_port, "/pang/ws/pub?channel=ca6s906pjh63qlu2ahh0&track=colink"))
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Websocket Connected.");

  servo_center();
}

void loop()
{
  client.poll();
  client.send("HeartBeat");
}