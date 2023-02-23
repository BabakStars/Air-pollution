#include<Wire.h>
#include"SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "MQ9_Air";
const char* password = "123456789";

ESP8266WebServer server (80);
IPAddress local_IP(192,168,1,151);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondryDNS(8,8,4,4);

SSD1306 display(0x3c,D2  ,D1  );    //SDA   SCK
float Sensor;
int timer = 0;
String SendSensor;
int temp=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(D0,OUTPUT);
    if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondryDNS))
  {
    
  }
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2,1);    //OFF
    delay(100);
    digitalWrite(2,0);    //ON
  }
  digitalWrite(2,0);
  display.init();
  Wire.begin();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(2, 5, "Starting...");
  display.flipScreenVertically();
  display.display();

  server.begin();
  
  digitalWrite(D0,1);
  delay(10000);
  display.clear();
  pinMode(D0,OUTPUT);
}


void loop() {
  L:
  server.handleClient();
  // put your main code here, to run repeatedly:
  temp++;
  digitalWrite(D0,1);
  while(timer < 55)
  {server.handleClient();
    display.clear();
    delay(1000);
    timer++;
    int updataTimre = 55-timer;
    String sendUpdata = String(updataTimre);
    display.setFont(ArialMT_Plain_10);
    String updata = "Update after " + sendUpdata + " seconds";
    display.drawString(2, 26, updata);
    display.display();
    display.setFont(ArialMT_Plain_24);
    display.drawString(2, 5, SendSensor + " %");
    display.display();
  }
  timer=0;
  
  display.clear();
  for(int cunter=0 ; cunter<10 ; cunter++)
  {
    Sensor += analogRead(A0);
    delay(50);
  }
  Sensor = Sensor/10;
  Sensor = (Sensor / 1023)*100;
  SendSensor = String(Sensor);
  
  if(Sensor >= 24)
  {
    String fan = "http://192.168.1.150/fan?status=on";
    HTTPClient http;
    http.begin(fan);
    int httpCode = http.GET();

      http.end();
      httpCode=0;
  }
  if(Sensor < 15)
  {
    String fan = "http://192.168.1.150/fan?status=off";
    HTTPClient http;
    http.begin(fan);
    int httpCode = http.GET();
   
      httpCode=0;
     http.end();

  }
  display.clear();
     display.setFont(ArialMT_Plain_24);
      display.drawString(2, 5, SendSensor + " %");
      display.display();
      delay(2000);
      if(temp > 5)
      {
        digitalWrite(D0,0);
      }
      display.drawString( 2, 26, "Resting");
      display.display();
        if(Sensor < 15)
        {
          display.setFont(ArialMT_Plain_10);
          display.drawString(2, 52, "Fan must be OFF");
          display.display();
        }
        if(Sensor >= 24)
        {
          display.setFont(ArialMT_Plain_10);
          display.drawString(2, 52, "Fan must be ON");
          display.display();
        }
      delay(50000);
  
}
