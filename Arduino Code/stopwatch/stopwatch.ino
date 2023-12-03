#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Stopwatch");
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const int SEN_1 = 4;  // Define the GPIO pin connected to the TRIG pin of the sensor
const int SEN_2 = 2;  // Define the GPIO pin connected to the ECHO pin of the sensor
const int sw = 15, buzzer = 13;
bool detect1 = 0, detect2 = 0, temp1 = 1, temp2 = 1, armed = 0, instance = 0;
int p_temp;
uint32_t timer = millis(), gap = millis(), s1 = millis();

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
}

void loop() {
  if (detect1 != temp1) {
    detect1 = temp1;
    if (detect1) s1 = millis();
    display_panel();
  }


  if (detect2 != temp2) {
    detect2 = temp2;
    display_panel();
  }


  // if (detect != temp) {
  //   temp = detect;
  //   display.invertDisplay(detect);
  //   if (armed) {
  //     if (detect) {
  //       if (millis() - gap > 30000) {
  //         bleKeyboard.write(KEY_PAGE_DOWN);
  //         armed = 0;
  //         text("   IDLE   ", 4, 24);
  //       }
  //     } else if (instance) {
  //       bleKeyboard.write(KEY_PAGE_UP);
  //       gap = millis();
  //       instance = 0;
  //     }
  //   }
  // }

  detect1 = digitalRead(SEN_1);
  detect2 = digitalRead(SEN_2);
  if (!digitalRead(sw) && !armed && detect1) {
    armed = 1;
    text("  ARMED  ", 11, 24);
    instance = 1;
  }
}

void display_panel() {
  display.clearDisplay();
  text("   IDLE   ", 4, 12);
  text(" " + String(detect1) + "      " + String(detect2) + " ",4,36);
}

void text(String s, byte x, byte y) {
  display.clearDisplay();
  display.setCursor(x, y);
  display.print(s);
  display.display();
}
