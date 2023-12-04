#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Stopwatch");
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const int SEN_1 = 32;  // Define the GPIO pin connected to the TRIG pin of the sensor
const int SEN_2 = 33;  // Define the GPIO pin connected to the ECHO pin of the sensor
const int sw = 15, buzzer = 14;
bool detect1 = 0, detect2 = 0, temp1 = 1, temp2 = 1, armed = 0, running = 0;
int p_temp;
uint32_t timer = millis(), gap = millis();

void setup() {
  Serial.begin(9600);
  pinMode(SEN_1, INPUT);
  pinMode(SEN_2, INPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  text("CONNECTING", 4, 24);
  bleKeyboard.begin();
  while (!bleKeyboard.isConnected())
    ;
  display.clearDisplay();
}

void loop() {
  if (detect1 != temp1) {
    temp1 = detect1;
    if (detect1) gap = millis();
    else if (armed) {
      armed = 0;
      running = 1;
      bleKeyboard.write(KEY_PAGE_UP);
      gap = millis();
    }
    display_panel();
  }


  if (detect2 != temp2) {
    temp2 = detect2;
    if (detect2 && running && millis() - gap > 10000) {
      running = 0;
      bleKeyboard.write(KEY_PAGE_DOWN);
    }
    display_panel();
  }

  detect1 = !digitalRead(SEN_1);
  detect2 = !digitalRead(SEN_2);

  if (!digitalRead(sw) && !armed && detect1) {
    armed = 1;
    display_panel();
  }
}

void display_panel() {
  display.clearDisplay();
  if (armed) text("  ARMED  ", 11, 12);
  else if (running) text(" RUNNING ", 11, 12);
  else text("   IDLE   ", 4, 12);
  text(" " + String(temp1) + "      " + String(temp2) + " ", 4, 36);
  display.display();
}

void text(String s, byte x, byte y) {
  display.setCursor(x, y);
  display.print(s);
}
