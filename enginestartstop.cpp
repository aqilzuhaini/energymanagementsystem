/* Name: Amirul Hakeem Bin Hanizan
ID: 22001871
*/

// Pin definitions
const int startStopButtonPin = 2;
const int throttleLedPin = 9;  // PWM pin for throttle LED
const int tempSensorPin = A0;  // Analog pin for temperature sensor
const int potentiometerPin = A1;  // Analog pin for potentiometer

// Variables
bool engineRunning = false;
int buttonState = 0;
int lastButtonState = 0;
float temperature = 0.0;
const float safeTempMin = 20.0;  // Minimum safe temperature in Celsius
const float safeTempMax = 70.0;  // Maximum safe temperature in Celsius

void setup() {
    // Initialize pins
    pinMode(startStopButtonPin, INPUT_PULLUP);
    pinMode(throttleLedPin, OUTPUT);
    pinMode(tempSensorPin, INPUT);
    pinMode(potentiometerPin, INPUT);
    Serial.begin(9600);  // For debugging
}

void loop() {
    // Read the temperature
    temperature = readTemperature(tempSensorPin);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    // Read the state of the start/stop button
    buttonState = digitalRead(startStopButtonPin);

    // Check if the button was pressed
    if (buttonState == LOW && lastButtonState == HIGH) {
        delay(50);  // Debounce delay
        buttonState = digitalRead(startStopButtonPin);  // Read button state again
        if (buttonState == LOW) {
            engineRunning = !engineRunning;  // Toggle engine state
            Serial.println(engineRunning ? "Engine started" : "Engine stopped");
        }
    }
    lastButtonState = buttonState;  // Save the current button state

    // Control the throttle (LED intensity) based on the engine state and temperature
    if (engineRunning && temperature >= safeTempMin && temperature <= safeTempMax) {
        int throttleValue = analogRead(potentiometerPin);  // Read potentiometer value
        int throttlePWM = map(throttleValue, 0, 1023, 0, 255);  // Map to PWM range
        analogWrite(throttleLedPin, throttlePWM);  // Adjust throttle LED brightness
        Serial.print("Engine running and throttle adjusted to PWM: ");
        Serial.println(throttlePWM);
    }
    else {
        analogWrite(throttleLedPin, 0);  // Turn off throttle LED
        Serial.println("Engine stopped or unsafe temperature.");
    }

    delay(100);  // Short delay for stability
}

// Function to read temperature from TMP36 sensor
float readTemperature(int sensorPin) {
    int reading = analogRead(sensorPin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureC = (voltage - 0.5) * 100;  // Convert voltage to temperature in Celsius
    return temperatureC;
}