#include "LCDControl.h"
#include "Images.h"
// #include "bluetooth.h"

#define BTN_PIN 10

int simpleDisplay = true;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading

void setup(void)
{

  startBluetooth();
  
  // set pinMode for buttons
  pinMode(BTN_PIN, INPUT);

  // initialize and clear screen
  LcdInitialise();
  LcdClear();

  // sample display  
  printBitmap(left_roundabout, 0, 0, 32, 6);
  printBitmap(left_hairpin, 38, 0, 32, 6);
  //printString("Rue Joliot-Curie", 0, 4);
}

void loop(void)
{

  /*
   * 
   * HANDLE BUTTON INPUT WITH DEBOUNCE
   * 
   */
   
  int reading = digitalRead(BTN_PIN); 
    
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        simpleDisplay = !simpleDisplay;
      }
    }
  }
  
  lastButtonState = reading;

  /////////////////////////////////////////

  if(simpleDisplay) {
    printString("Aff. simple", 0, 5);
  }
  else {
    printString("Aff. detail", 0, 5);
  }

  /*
   * 
   * Display
   * 
   */


   if(simpleDisplay) { // AFFICHAGE SIMPLE
    
   }
   else {              // AFFICHAGE DETAILLE
    
   }

   ////////////////////////////////////////

  /*
   * 
   * Bluetooth Communication
   * 
   */
  
  readSerialBluetooth();
  processBluetoothCommand();

  //////////////////////////////////////////
}


