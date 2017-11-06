#include "LCDControl.h"
#include "Images.h"

#define BTN_BT 8
#define BTN_AFF 9

int state = 0;

void setup(void)
{
  
  // set pinMode for buttons
  pinMode(BTN_BT, INPUT);
  pinMode(BTN_AFF, INPUT);

  // initialize and clear screen
  LcdInitialise();
  LcdClear();

  // sample display  
  printBitmap(left_roundabout, 0, 0, 38, 4);
  printBitmap(left, 38, 0, 35, 5);
  printString("Rue Joliot-Curie", 0, 4);


}

void loop(void)
{

}


