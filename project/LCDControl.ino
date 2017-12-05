// define screen's pins

#define PIN_SCE   7
#define PIN_RESET 6
#define PIN_DC    5
#define PIN_SDIN  4
#define PIN_SCLK  3

// define useful constants related to the display
#define LCD_X     84
#define LCD_Y     48
#define LCD_CMD   0
#define LCD_C     LOW
#define LCD_D     HIGH

// include ascii hex symbols
#include "ascii.h"

// clears the screen
void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00); // write blank bits everywhere
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE,   OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC,    OUTPUT);
  pinMode(PIN_SDIN,  OUTPUT);
  pinMode(PIN_SCLK,  OUTPUT);

  digitalWrite(PIN_RESET, LOW);
 // delay(1);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
  LcdWrite( LCD_CMD, 0xA8 );  // Set LCD Vop (Contrast). //B1
  LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite( LCD_CMD, 0x0C );  // LCD in normal mode. 0x0d for inverse
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data); // write data to selected pin
  digitalWrite(PIN_SCE, HIGH);
}

// change cursor to the (x, y*8) pixel
void goTo(int x, int y)
{
  LcdWrite(LCD_CMD, 0x40 | y);
  LcdWrite(LCD_CMD, 0x80 | x);
}

// prints a bimap starting at the (x, y*8) pixel with given width and height
void printBitmap(const byte* bmp, int x, int y, int w, int h)
{
  for(int j = 0; j < h; j++) {
    for(int i = 0; i < w; i++) {
      goTo(x+i, y+j);
      LcdWrite(LCD_D, bmp[i + j*w]); // write hex symbol
    }
  }
}

// print character from ascii table
void printCharacter(char character)
{
  LcdWrite(LCD_D, 0x00); // blank line at the left
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00); // blank line at the right
}

// print string at given position
void printString(char* string, int x, int y)
{
  goTo(x, y);
  while (*string)
  {
    printCharacter(*string++);
  }
}

const unsigned char* digitMapper[10] = {
  number_0,
  number_1,
  number_2,
  number_3,
  number_4,
  number_5,
  number_6,
  number_7,
  number_8,
  number_9,
};

void printDigit(int digit, int x, int y) {
  printBitmap(digitMapper[digit], x, y, 17, 4);
}

void printNumber(int num, int x, int y) {
  int rightDigit = num % 10;
  int middleDigit = (num / 10) % 10;
  int leftDigit = (num / 100) % 10;

  printDigit(rightDigit, x + 34, y);
  if (middleDigit != 0 || leftDigit != 0) {
    printDigit(middleDigit, x + 17, y);
  }
  if (leftDigit != 0) {
    printDigit(leftDigit, x, y);
  }
}

