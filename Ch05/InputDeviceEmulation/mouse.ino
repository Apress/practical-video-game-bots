#include <Mouse.h>

void setup()
{
  Serial.begin(9600);
  Mouse.begin();
}

void click(signed char x, signed char y, char button)
{
  Mouse.move(x, y);
  Mouse.click(button);
}

void loop()
{
  static const char PREAMBLE = 0xDC;
  static const uint8_t BUFFER_SIZE = 4;

  if (Serial.available() > 0)
  {
    char buffer[BUFFER_SIZE] = {0};
    uint8_t readBytes = Serial.readBytes(buffer, BUFFER_SIZE);

    if (readBytes != BUFFER_SIZE)
      return;

    if (buffer[0] != PREAMBLE)
      return;

   click(buffer[1], buffer[2], buffer[3]);
  }  
}
