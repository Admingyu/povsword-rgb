#include <ESP8266WebServer.h>
// #include <FS.h>
#include <LittleFS.h>


ESP8266WebServer server ( 80 );

String color = "00FFFF";
String jscolor = "";
int r = 0;
int g = 255;
int b = 255;
int brightness = 90;
bool brightnessChanged = false;

const int led = 13;
String message = "";
bool messageChanged = true;


void handleRoot() {
  String messageSafe;
  messageSafe.reserve(message.length());
  for (size_t pos = 0; pos != message.length(); ++pos) {
    switch (message[pos]) {
      case '&':  messageSafe += "&amp;";       break;
      case '\"': messageSafe += "&quot;";      break;
      case '\'': messageSafe += "&apos;";      break;
      case '<':  messageSafe += "&lt;";        break;
      case '>':  messageSafe += "&gt;";        break;
      default:   messageSafe += message[pos];  break;
    }
  }
  String toSend = "<html><head><script src=\"jscolor.min.js\"></script><title>ESP8266 Demo</title><style>body{background-color:#ccc;font-family:Arial,Helvetica,Sans-Serif;Color:#008}#picker{width:200px;height:200px;display:inline-block;}#slide{width:30px;height:200px;display:inline-block;}</style></head><body><h1>Hello from ESP8266!</h1><form action=\"\"method=\"post\"><label for=\"name\">Message:</label><input id=\"name\" name=\"name\" value=\"" + messageSafe + "\"><br><label for=\"color\">Color:</label><input id=\"color\" name=\"color\" class=\"jscolor\"  value=\"" + color + "\"><div id=\"picker\"></div><div id=\"slide\"></div><script type=\"text/javascript\">ColorPicker(document.getElementById('slide'),document.getElementById('picker'),function(hex){document.getElementById('color').value=hex;document.getElementById('color').style.backgroundColor=hex}).setHex('#'+document.getElementById('color').value);document.getElementById('color').style.backgroundColor='#'+document.getElementById('color').value;</script><br><label for=\"brightness\">Brightness:</label><input id=\"brightness\" name=\"brightness\" value=\"" + brightness + "\" min=\"0\" max=\"255\" type=\"number\"><br><input type=\"submit\" value=\"Save\"></form></body></html>";
  server.send ( 200, "text/html", toSend );
}

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}

void getData() {
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if (server.argName ( i ) == "name") {
      message = server.arg ( i );
      messageChanged = true;
    } else if (server.argName ( i ) == "color") {
      color = server.arg ( i );
      Serial.println(color);
      if (color.charAt(0) == '#') {
        color = color.substring(1);
      }
      Serial.println(color);
      Serial.println(color.length());
      if (color.length() != 6) continue;
      long number = (long) strtol(color.c_str(), NULL, 16);
      r = number >> 16;
      g = number >> 8 & 0xFF;
      b = number & 0xFF;
    } else if (server.argName ( i ) == "brightness") {
      int br = server.arg(i).toInt();
      if (br >= 0 && br <= 255) brightness = br;
      brightnessChanged = true;
    }
  }
  Serial.println(message);
  handleRoot();
}

void startServer(){
  LittleFS.begin();
  File file = LittleFS.open("/jscolor.min.js", "r");
  jscolor = file.readString();
  file.close();
  server.on ( "/", HTTP_GET, handleRoot );
  server.on ( "/", HTTP_POST, getData );
  server.on ( "/jscolor.min.js", HTTP_GET, []() {
  server.send ( 200, "application/javascript", jscolor );
  });
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}
