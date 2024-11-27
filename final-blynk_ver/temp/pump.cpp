#include <Arduino.h>

unsigned long lastDecounceTime = 0;
unsigned long debounceDelay = 100;
int buttonState = LOW;
int lastButtonState = LOW;

int pumpPinGlobal = 33;

bool isPumping = false;
unsigned long pumpStartTime = 0;
unsigned long pumpDuration = 5000; 

void initPump(int buttonPin, int pumpPin)
{
    pinMode(buttonPin, INPUT);
    pinMode(pumpPin, OUTPUT);
    digitalWrite(pumpPin, LOW);

    pumpPinGlobal = pumpPin;
}



void startPump(){
    if (!isPumping){
        isPumping = true;
        pumpStartTime = millis();
        digitalWrite(pumpPinGlobal, HIGH);
        Serial.println("Pumping water ...");
    }
}

void pumpControl(int buttonPin, int pumpPin){
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState){
        lastDecounceTime = millis();
    }

    if (millis()-lastDecounceTime > debounceDelay){
        if (reading != buttonState){
            buttonState = reading;
            if (buttonState == HIGH){
                startPump();
            }
        }
    }

    lastButtonState = reading;
}

//check pump status if it passed 5 seconds
void handlePumpState(unsigned long inputPumpDuration){
    pumpDuration = inputPumpDuration;

    if (isPumping && millis() - pumpStartTime > pumpDuration){
        isPumping = false;
        digitalWrite(pumpPinGlobal, LOW);
        Serial.println("Water pumped");
    }
}