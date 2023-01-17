#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <main.h>
#include <handler.h>

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  WiFi.softAP("LED Matrix", "12345678");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  message = "IP Address: ";
  message += WiFi.localIP().toString();

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  // SPIFFS.begin();
  // File file = SPIFFS.open("/jscolor.min.js", "r");
  // jscolor = file.readString();
  // file.close();
  // server.on ( "/", HTTP_GET, handleRoot );
  // server.on ( "/", HTTP_POST, getData );
  // server.on ( "/jscolor.min.js", HTTP_GET, []() {
  //   server.send ( 200, "application/javascript", jscolor );
  // });
  // server.onNotFound ( handleNotFound );
  // server.begin();
  // Serial.println ( "HTTP server started" );
  randomSeed(analogRead(0));
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
}

int x = matrix.width();

void loop(void)
{
  server.handleClient();
  if (messageChanged)
  {
    x = 1;
    messageChanged = false;
    Serial.println(message);
  }
  yield();

  if (brightnessChanged)
  {
    matrix.setBrightness(brightness);
  }

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.setTextColor(matrix.Color(r, g, b));
  matrix.print(message);
  if (x < ((int)message.length()) * -6)
  {
    Serial.print(x);
    Serial.println(((int)message.length()) * -6);
    x =1;
    Serial.print('finished one complete roll');
  }
  x--;
  matrix.show();
  delay(50);
}