#include "LCDControl.h"
#include "Images.h"
#include "bluetooth.h"


/*
 * STATE VARIABLE
 */

// TODO: change this bool:
int hasReceivedBluetoothInfo = true;
int simpleDisplay = true;
int currentAngle = 57;
int currentDistance = 0;
int currentError = 0;
char currentType = '\0';
int currentExit = 0;
char exitText [8];
int timeToDestination = 12;
int distanceToDestination = 7;
char arrivalText [13];
char turnText [37];
char nextStreet [13] = "Joliot Curie";

/*
 * BUTTON VARIABLES
 */
#define BTN_PIN 1

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

  if (hasReceivedBluetoothInfo) {
   if(simpleDisplay) {
      printBitmap(chooseIntersectionToDisplay(), 0, 0, 32, 6);
      printString(getExitInfo(), 33, 0);
      printNumber(getFormattedDistance(), 33, 1);
      printString(getDistanceUnit(), 33, 5);
   }
   else {
    printString("Arrivee dans", 0, 0);
    printString(getArrivalText(), 0, 1);
    printString(getTurnText(), 0, 3);
   }
  } else {
    // TODO: add bluetooth logo
    printBitmap(bluetooth_logo, 0, 0, 32, 6);
    printString("En ", 33, 1);
    printString("attente", 33, 2);
    printString("...", 33, 3);
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


