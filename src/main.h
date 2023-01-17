#include <ESP8266mDNS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


#define PIN D6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);
const char *ssid = "RGBSword";
const char *password = "12345678";