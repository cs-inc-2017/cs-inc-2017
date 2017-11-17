#include "LCDControl.h"
#include "Images.h"
#include "bluetooth.h"


/*
 * STATE VARIABLE
 */
int simpleDisplay = true;
int currentAngle = 0;
int currentDistance = 0;
int currentError = 0;
int currentType = 0;
int currentExit = 0;

/*
 * BUTTON VARIABLES
 */
#define BTN_PIN 10

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading


/*
 * 
 * SETUP FUNCTION
 * 
 */
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

/*
 * 
 * LOOP FUNCTION
 * 
 */
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

 

  /*
   * 
   * Display
   * 
   */

   if(simpleDisplay) {
    printString("Aff. simple", 0, 5);
   }
   else {
     printString("Aff. detail", 0, 5);
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


