#include "LCDControl.h"

void setup(void)
{

  LcdInitialise();
  LcdClear();
  
  printBitmap(left, 0, 0, 38, 4);
  

}

void loop(void)
{
  
}

