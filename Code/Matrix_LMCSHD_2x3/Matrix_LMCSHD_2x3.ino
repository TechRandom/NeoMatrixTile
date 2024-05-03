#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>

#define PIN 21
#define BRIGHTNESS 30     // Out of 255

#define mw 48
#define mh 32
#define NUMMATRIX (mw*mh)

CRGB leds[NUMMATRIX];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 16, 16, 3, 2, 
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_BOTTOM + NEO_TILE_LEFT +  NEO_TILE_COLUMNS + NEO_TILE_ZIGZAG);

uint16_t rawData[NUMMATRIX * 2]; // Two Bytes per pixel

void setup() {
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUMMATRIX);
  Serial.begin(2000000);
  matrix->setBrightness(BRIGHTNESS);
}

void loop() {
  uint8_t data = Serial.read();
  if (data == 0x05){
    uint8_t width = mw, height = mh;
    Serial.println(width);
    Serial.println(height);
  }
  else if (data == 0x42){
    Serial.readBytes((uint8_t *)rawData, NUMMATRIX * 2);

    // Swap every two bytes
    for (int i = 0; i < NUMMATRIX; i++) {
      // Swap the bytes
      rawData[i] = ((rawData[i] & 0xFF) << 8) | ((rawData[i] & 0xFF00) >> 8);
    }

    matrix->drawRGBBitmap(0, 0, rawData, 48, 32);
    matrix->show();
    Serial.write(0x06); //acknowledge
  }
}
