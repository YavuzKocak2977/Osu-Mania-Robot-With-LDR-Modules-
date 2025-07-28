// This code made by Yavuz Koçak
// Used By ESP32-S3-DEVKIT
#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h> // Cant use Keyboard.h regularly, it wont work with for esp's

USBHIDKeyboard keyboard;

const int LDR1_PIN = 4;
const int LDR2_PIN = 5;
const int LDR3_PIN = 6;
const int LDR4_PIN = 7;

const int ON_OFF_PIN = 18;
const int GREEN_LIGHT = 17;
const int RED_LIGHT = 8;

// Helper variables for the toggle logic
bool system_is_on = false;
bool last_touch_state = LOW;

void setup() {
  Serial.begin(115200);
  keyboard.begin();
  USB.begin();

  pinMode(LDR1_PIN, INPUT);
  pinMode(LDR2_PIN, INPUT);
  pinMode(LDR3_PIN, INPUT);
  pinMode(LDR4_PIN, INPUT);

  pinMode(ON_OFF_PIN, INPUT);
  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(RED_LIGHT, OUTPUT);
}

void loop() {
  // Toggle Switch Logic
  bool current_touch_state = digitalRead(ON_OFF_PIN);

  if (current_touch_state == HIGH && last_touch_state == LOW) {
    system_is_on = !system_is_on;
    delay(50);
  }
  last_touch_state = current_touch_state;

  if (system_is_on) {
    analogWrite(GREEN_LIGHT, 200);
    analogWrite(RED_LIGHT, 0);
  } else {
    analogWrite(GREEN_LIGHT, 0);
    analogWrite(RED_LIGHT, 200);
  }

  // Keyboard Logic
  static bool d_KEY = false;
  static bool f_KEY = false;
  static bool j_KEY = false;
  static bool k_KEY = false;

  if (system_is_on) {
    int LDR1_VALUE = analogRead(LDR1_PIN);
    int LDR2_VALUE = analogRead(LDR2_PIN);
    int LDR3_VALUE = analogRead(LDR3_PIN);
    int LDR4_VALUE = analogRead(LDR4_PIN);

    if (LDR1_VALUE < 150 && d_KEY == false ) { // This mean ldr value gets the value and have to press, and our key is dosent press now
      keyboard.press ('d');
      d_KEY = true;
    }
    else if (LDR1_VALUE >= 150 && d_KEY == true ) { // This mean ldr wont reciev any value and d key is pressed now have to get release.
      keyboard.release ('d');
      d_KEY = false;
    }

    if (LDR2_VALUE < 150 && f_KEY == false ) {
      keyboard.press ('f');
      f_KEY = true;
    } else if (LDR2_VALUE >= 150 && f_KEY == true ) {
      keyboard.release ('f');
      f_KEY = false;
    }

    if (LDR3_VALUE < 150 && j_KEY == false ) {
      keyboard.press ('j');
      j_KEY = true;
    } else if (LDR3_VALUE >= 150 && j_KEY == true ) {
      keyboard.release ('j');
      j_KEY = false;
    }

    if (LDR4_VALUE < 150 && k_KEY == false ) {
      keyboard.press ('k');
      k_KEY = true;
    } else if (LDR4_VALUE >= 150 && k_KEY == true ) {
      keyboard.release ('k');
      k_KEY = false;
    }
  } else {
    keyboard.releaseAll(); // If the system is turned off, make sure all keys are released.
    d_KEY = false;
    f_KEY = false;
    j_KEY = false;
    k_KEY = false;
  }
  delay(1);
}