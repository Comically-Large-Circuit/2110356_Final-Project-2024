#include <Arduino.h>

unsigned long lastDebounceTime = 0;  // Fixed typo in variable name
unsigned long debounceDelay = 100;
int buttonState = LOW;
int lastButtonState = LOW;

int pumpPinGlobal = 33;

bool isPumping = false;
unsigned long pumpStartTime = 0;
unsigned long pumpDuration = 5000; // Default to 5 seconds

void initPump(int buttonPin, int pumpPin)
{
    pinMode(buttonPin, INPUT);
    pinMode(pumpPin, OUTPUT);
    digitalWrite(pumpPin, LOW);

    pumpPinGlobal = pumpPin;
}

void startPump()
{
    if (!isPumping)
    {
        isPumping = true;
        pumpStartTime = millis();
        digitalWrite(pumpPinGlobal, HIGH);
        Serial.println("Pumping water ...");
        Serial.print("Pump duration: ");
        Serial.print(pumpStartTime);
        Serial.println(" ms");
    }
}

// Check pump status to see if it has run for the specified duration
void handlePumpState()
{
    if (isPumping && (millis() - pumpStartTime) > pumpDuration)
    {
        isPumping = false;
        digitalWrite(pumpPinGlobal, LOW);
        Serial.println("Water pumped");
    }
}

void pumpControl(int buttonPin, int pumpPin, int pump_duration)
{
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime > debounceDelay)
    {
        if (reading != buttonState)
        {
            buttonState = reading;
            if (buttonState == HIGH)
            {
                pumpDuration = pump_duration; // Update global pumpDuration
                startPump();
            }
        }
    }
    handlePumpState(); // No need to pass pump_duration here
    lastButtonState = reading;
}
