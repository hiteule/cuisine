#include <avr/sleep.h>
#include <TM1637Display.h>

#define PIN_MICROWAVE_LED A0
#define PIN_MICROWAVE_BTN 2
#define PIN_MICROWAVE_MOSFET A2

#define PIN_OVEN_LED A1
#define PIN_OVEN_BTN 3
#define PIN_OVEN_MOSFET A3

#define TM1637_CLK 4
#define TM1637_DIO 5

#define PIN_BUZZER 8
#define MICROWAVE_TIME 5

volatile int microwaveTime = 0;
int microwaveTimeMin = 0;
volatile bool ovenPower = false;
volatile unsigned long lastInterrupt;
TM1637Display display(TM1637_CLK, TM1637_DIO);

void setup() {
  pinMode(PIN_MICROWAVE_LED, OUTPUT);
  pinMode(PIN_MICROWAVE_BTN, INPUT_PULLUP);
  pinMode(PIN_MICROWAVE_MOSFET, OUTPUT);
  pinMode(PIN_OVEN_LED, OUTPUT);
  pinMode(PIN_OVEN_BTN, INPUT_PULLUP);
  pinMode(PIN_OVEN_MOSFET, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  display.setBrightness(1, false);

  analogWrite(PIN_MICROWAVE_LED, 0);
  analogWrite(PIN_MICROWAVE_MOSFET, 0);
  analogWrite(PIN_OVEN_LED, 0);
  analogWrite(PIN_OVEN_MOSFET, 0);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  attachInterrupt(digitalPinToInterrupt(PIN_MICROWAVE_BTN), addMicrowaveTime, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_OVEN_BTN), switchOvenPower, RISING);

  sleep_cpu();
}

void loop() {
  analogWrite(PIN_MICROWAVE_LED, 0);

  if (0 < microwaveTime) {
    microwaveStart();

    do {
      printMicrowaveTime();
      delay(1000);
      microwaveTime--;
    } while(0 < microwaveTime);

    microwaveStop();

    return;
  }

  sleep_cpu();
}

void addMicrowaveTime() {
  sleep_disable();

  if((millis() - lastInterrupt) < 600) {
    return;
  }

  microwaveTime += MICROWAVE_TIME;
  printMicrowaveTime();
  lastInterrupt = millis();
}

void printMicrowaveTime() {
  microwaveTimeMin = floor(microwaveTime / 60);
  display.setBrightness(1, true);
  display.showNumberDecEx((microwaveTime - (microwaveTimeMin * 60)) + (microwaveTimeMin * 100), (0x80 >> 1), true, 4, 0);
}

void microwaveStart() {
  analogWrite(PIN_MICROWAVE_LED, 255);
  analogWrite(PIN_MICROWAVE_MOSFET, 255);
  tone(PIN_BUZZER, 100);
}

void microwaveStop() {
  analogWrite(PIN_MICROWAVE_MOSFET, 0);
  noTone(PIN_BUZZER);

  const uint8_t segStop[] = {
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,         // S
    SEG_D | SEG_E | SEG_F | SEG_G,                 // t
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G          // P
  };

  for (int i = 0; i < 6; i++) {
    if (0 == i%2) {
      analogWrite(PIN_MICROWAVE_LED, 255);
      tone(PIN_BUZZER, 2000);
      display.setBrightness(1, true);
      display.setSegments(segStop);
    } else {
      analogWrite(PIN_MICROWAVE_LED, 0);
      noTone(PIN_BUZZER);
      display.setBrightness(1, false);
      display.setSegments(segStop);
    }

    delay(500);
  }
}

void switchOvenPower() {
  sleep_disable();

  if((millis() - lastInterrupt) < 1000) {
    return;
  }

  ovenPower = !ovenPower;

  analogWrite(PIN_OVEN_LED, ovenPower ? 255 : 0);
  analogWrite(PIN_OVEN_MOSFET, ovenPower ? 255 : 0);

  lastInterrupt = millis();
}
