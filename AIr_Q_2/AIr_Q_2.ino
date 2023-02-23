#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server (80);
IPAddress local_IP(192,168,1,150);
IPAddress subnet(255,255,255,0);

void setup() {

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP,local_IP,subnet);
  WiFi.softAP("MQ9_Air","123456789");
  pinMode(D0,OUTPUT);
  digitalWrite(D0,1);

  server.on("/fan",action);
  server.begin();
}
void action() {
  String S = server.arg("status");
  if(S == "on")
  {
    digitalWrite(D0,0);
    server.send(200,"text/plain","Fan is ON");
  }

  if(S == "off")
  {
    digitalWrite(D0,1);
    server.send(200,"text/plain","Fan is OFF");
  }
  delay(10);
}
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
