#include "LCDControl.h"
#include "Images.h"
#include "bluetooth.h"

/*
 * STATE VARIABLE
 */

int hasReceivedBluetoothInfo = false;
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
unsigned long previousMillis = 0;
int displayError = false;

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

  printBitmap(bike_logo, 0, 0, 84, 5);
  printString("FindYourWay", 0, 5);
  delay(3000);
  LcdClear();
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

 

  // Prepare error display if needed
  if (currentError > 0) {
    if (currentType == 'r') {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        if (displayError) {
          displayError = false;
        } else {
          displayError = true;
        }
      }
    } else {
      displayError = true;
    }
  } else {
    displayError = false;
  }

  /*
   * 
   * Display
   * 
   */

  if (hasReceivedBluetoothInfo) {
   if(simpleDisplay) {
      printBitmap(chooseIntersectionToDisplay(), 0, 0, 32, 6);
      if (displayError) {
        printString(getErrorText(), 33, 0);
      } else {
        printString(getExitInfo(), 33, 0);
      }
      printNumber(getFormattedDistance(), 33, 1);
      printString(getDistanceUnit(), 33, 5);
   }
   else {
    printString("Arrivee dans", 0, 0);
    printString(getArrivalText(), 0, 1);
    printString(getTurnText(), 0, 3);
   }
  } else {
      printBitmap(bluetooth_logo, 0, 0, 32, 6);
      printString("  En   ", 33, 2);
      printString("attente", 33, 3);
      printString("  ...  ", 33, 4);
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


