#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ESP32 Media Knob");

const int clkPin = 32;
const int dtPin  = 33;
const int swPin  = 25;

int lastClk = HIGH;
bool lastButton = HIGH;
bool trackChanged = false;

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
      if (buttonState == LOW && !trackChanged) { // Only if track has NOT been changed yet
        if (digitalRead(dtPin) != clkState) {
          bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
          Serial.println("NEXT TRACK");
        } else {
          bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
          Serial.println("PREVIOUS TRACK");
        }
        trackChanged = true; // Lock until button is released
      } else if (buttonState == HIGH) {
        // Volume (no lock)
        if (digitalRead(dtPin) != clkState) {
          bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
          Serial.println("VOL UP");
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
      delay(50);
      if (digitalRead(swPin) == LOW) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        Serial.println("PLAY/PAUSE");
      }
    }
    // If button is released, allow new track change
    if (lastButton == LOW && buttonState == HIGH) {
      trackChanged = false; // <-- Reset when button is released
      Serial.println("trackChange RESET");
      delay(700); // Increase delay to avoid unconsciously editing the volume
    }
    lastButton = buttonState;
  }
}