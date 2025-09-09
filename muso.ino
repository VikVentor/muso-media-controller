#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ESP32 Media Knob");

const int clkPin = 32;
const int dtPin  = 33;
const int swPin  = 25;

int lastClk = HIGH;
bool lastButton = HIGH;

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();

  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
}

void loop() {
  if (bleKeyboard.isConnected()) {
    bool buttonState = digitalRead(swPin);

    // --- Encoder rotation ---
    int clkState = digitalRead(clkPin);
    if (clkState != lastClk) {
      if (digitalRead(dtPin) != clkState) {
        if (buttonState == LOW) {
          bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
          Serial.println("NEXT TRACK");
        } else {
          bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
          Serial.println("VOL UP");
        }
      } else {
        if (buttonState == LOW) {
          bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
          Serial.println("PREVIOUS TRACK");
        } else {
          bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
          Serial.println("VOL DOWN");
        }
      }
      delay(5);
    }
    lastClk = clkState;

    // --- Button press (play/pause) ---
    if (lastButton == HIGH && buttonState == LOW) {
      // debounce delay
      delay(50);
      if (digitalRead(swPin) == LOW) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        Serial.println("PLAY/PAUSE");
        delay(300); // debounce
      }
    }
    lastButton = buttonState;
  }
}