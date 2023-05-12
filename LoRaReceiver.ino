#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define ss 25
#define rst 14
#define dio0 2
void setup() {
  Serial.begin(9600);
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(866E6);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println(LoRa.packetRssi());
    int i =  map(LoRa.packetRssi(), -200, -15, 10, 60);
    display.clearDisplay();
    display.setCursor(32, 3);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(LoRa.packetRssi());
    display.setCursor(0, 10);
    display.drawRect(5, 20, 120, 10, SSD1306_WHITE);
    display.fillRect(5, 20, i*2, 10, SSD1306_WHITE);
    display.display();
  }
}
