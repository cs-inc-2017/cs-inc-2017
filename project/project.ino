#include "LCDControl.h"
#include "Images.h"

void setup(void)
{

  // initialize and clear screen
  LcdInitialise();
  LcdClear();
  
  // sample display  
  printBitmap(left_roundabout, 0, 0, 38, 4);
  printBitmap(left, 38, 0, 35, 5);
  printString("Bonjour", 0, 4);
  

}

void loop(void)
{
  
}

