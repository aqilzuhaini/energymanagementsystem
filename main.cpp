#define LED1_PIN 11
#define LED2_PIN 12
#define BUTTON_PIN 3
#define LED3_PIN 0
#define BUZZER_PIN 13
#define PMOSFET_PIN 9  
#define BUTTON2_PIN 2

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define MAX_TEMP 110  // Set maximum temperature to 110°C

int melody[] = {
  NOTE_G5, NOTE_E5, NOTE_G5, NOTE_G5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_G5, 
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_G5, NOTE_D5, NOTE_D5, 
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5
};

int noteDurations[] = {
  4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

int noteIndex = 0; // Index to track current note

bool lightBulbOn = false; // State of the light bulb
int buttonState2;
int lastButtonState2 = HIGH; // Initial state of button
int pwmValue = 0;  // Variable to store the current PWM value for the PMOSFET_PIN

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(PMOSFET_PIN, OUTPUT);  // Changed to PMOSFET_PIN
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  digitalWrite(PMOSFET_PIN, HIGH); // Ensure the light bulb is off initially (HIGH for PMOSFET)
  analogWrite(LED3_PIN, 255);  // Turn on LED3 at maximum brightness
  Serial.begin(9600);  // For debugging
}

void loop() {
  // Debounce for BUTTON_PIN
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);  // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {
      static int state = 0;  // Use static to maintain state across function calls
      state = (state + 1) % 3;  // Cycle through the states 0, 1, 2

      switch (state) {
        case 0:
          analogWrite(LED1_PIN, 0);  // Turn off LED1
          analogWrite(LED2_PIN, 0);  // Turn off LED2
          break;
        case 1:
          analogWrite(LED1_PIN, 255);  // Turn on LED1 at maximum brightness
          analogWrite(LED2_PIN, 0);    // Turn off LED2
          break;
        case 2:
          analogWrite(LED1_PIN, 0);    // Turn off LED1
          analogWrite(LED2_PIN, 255);  // Turn on LED2 at maximum brightness
          break;
      }

      while (digitalRead(BUTTON_PIN) == LOW) {
        // Wait for button release to avoid multiple toggles
      }
    }
  }

  // Debounce for BUTTON2_PIN
  buttonState2 = digitalRead(BUTTON2_PIN);
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {
    delay(50);  // Debounce delay
    buttonState2 = digitalRead(BUTTON2_PIN);  // Read button state again
    if (buttonState2 == LOW) {
      lightBulbOn = !lightBulbOn;  // Toggle light bulb state
      Serial.println(lightBulbOn ? "Light bulb on" : "Light bulb off");
      digitalWrite(PMOSFET_PIN, lightBulbOn ? LOW : HIGH);  // Control PMOSFET

      // Update the PWM value for the PMOSFET_PIN
      pwmValue = lightBulbOn ? 255 : 0;  // Example: full brightness or off
      Serial.print("PWM Value set to PMOSFET_PIN: ");
      Serial.println(pwmValue);
    }
  }
  lastButtonState2 = buttonState2;  // Save the current button state

  // Temperature in Celsius
  int reading = analogRead(A0); // Read temperature from temp sensor
  float voltage = reading * 5.0 / 1024.0; // Convert into voltage
  float celsius = (voltage - 0.5) * 100; // Voltage into Celsius

  // If temperature is above max temperature
  if (celsius >= MAX_TEMP) {
    playBarneySong();
  } else {
    noTone(BUZZER_PIN); // Stop the tone if temperature is below maxTemp
  }

  // Print the current PWM value being used
  Serial.print("Current PWM Value: ");
  Serial.println(pwmValue);
}

void playBarneySong() {
  // Play the Barney song melody
  int noteDuration = 1000 / noteDurations[noteIndex];
  tone(BUZZER_PIN, melody[noteIndex], noteDuration);
  delay(noteDuration * 1.30); // Add delay between notes
  noTone(BUZZER_PIN); // Stop the tone
  
  // Move to the next note
  noteIndex++;
  if (noteIndex >= sizeof(melody) / sizeof(melody[0])) {
    noteIndex = 0; // Loop back to the start of the melody
  }
}