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
  // printBitmap(left_roundabout, 0, 0, 32, 6);
  // printBitmap(left_hairpin, 38, 0, 32, 6);
  // printString("Rue Joliot-Curie", 0, 4);
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
        LcdClear();
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
      printBitmap(left_hairpin, 0, 0, 32, 6);
      printNumber(950, 33, 1);
      printString("metres", 41, 5);
   }
   else {
    printString("Aff. detail", 0, 5);

    char dst [4];
    sprintf (dst, "%03i", currentDistance);
    printString(dst, 0, 0);

    char err [4];
    sprintf (err, "%03i", currentError);
    printString(err, 0, 1);

    char ext [4];
    sprintf (ext, "%03i", currentExit);
    printString(ext, 0, 2);

    char agl [4];
    sprintf (agl, "%03i", currentAngle);
    printString(agl, 0, 3);
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


