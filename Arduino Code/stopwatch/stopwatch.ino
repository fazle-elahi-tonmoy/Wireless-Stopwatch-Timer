#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Stopwatch");
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const int TRIG_PIN = 18;  // Define the GPIO pin connected to the TRIG pin of the sensor
const int ECHO_PIN = 19;  // Define the GPIO pin connected to the ECHO pin of the sensor
const int sw = 15;
bool detect = 0, temp = 1, armed = 0, instance = 0;
int p_temp;
uint32_t timer = millis(), gap = millis();

void setup() {
  Serial.begin(9600);
  bleKeyboard.begin();
  while (!bleKeyboard.isConnected())
    ;
  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG pin as an OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Set ECHO pin as an INPUT
  pinMode(sw, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  text("   IDLE   ", 4, 24);
}




void loop() {
  if (detect != temp) {
    temp = detect;
    display.invertDisplay(detect);
    if (armed) {
      if (detect) {
        if (millis() - gap > 30000) {
          bleKeyboard.write(KEY_PAGE_DOWN);
          armed = 0;
          text("   IDLE   ", 4, 24);
        }
      } 
      else if (instance){
        bleKeyboard.write(KEY_PAGE_UP);
        gap = millis();
        instance = 0;
      }
    }
  }
  detect = sonar_read();
  if (!digitalRead(sw) && !armed && temp) {
    armed = 1;
    text("  ARMED  ", 11, 24);
    instance = 1;
  }
}

void text(String s, byte x, byte y) {
  display.clearDisplay();
  display.setCursor(x, y);
  display.print(s);
  display.display();
}
