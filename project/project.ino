#include "LCDControl.h"
#include "Images.h"
// #include "bluetooth.h"

#define BTN_BT 10
#define BTN_AFF 11

int state = 0;

int BTButtonState = LOW;
int lastBTButtonState = LOW;
int lastBTDebounceTime = 0;
int AFFButtonState = LOW;
int lastAFFButtonState = LOW;
int lastAFFDebounceTime = 0;

int debounceDelay = 50;

void setup(void)
{

  startBluetooth();
  
  // set pinMode for buttons
  pinMode(BTN_BT, INPUT);
  pinMode(BTN_AFF, INPUT);

  // initialize and clear screen
  LcdInitialise();
  LcdClear();

  // sample display  
 // printBitmap(left_roundabout, 0, 0, 38, 4);
  printBitmap(left_hairpin, 38, 0, 32, 6);
  //printString("Rue Joliot-Curie", 0, 4);


}

void loop(void)
{
  readSerialBluetooth();
  processBluetoothCommand();
  int BTButtonValue = digitalRead(BTN_BT);
  
  if(BTButtonValue != lastBTButtonState) {
    // lastDebounceTime = millis();
  }
  
  if(BTButtonValue == HIGH) {
    //printString("Rue Joliot-Curie", 0, 4);
  }
  else {
    //printString("Rue De Saclayyyy", 0, 4);
  }
}


