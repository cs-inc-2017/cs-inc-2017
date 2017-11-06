#include "LCDControl.h"
#include "Images.h"

void setup(void)
{

  LcdInitialise();
  LcdClear();
  
  printBitmap(left_roundabout, 0, 0, 38, 4);
  printBitmap(left, 38, 0, 35, 5);
  

}

void loop(void)
{
  
}

