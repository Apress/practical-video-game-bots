#include <Mouse.h>
#include <Keyboard.h>

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
}

void pressKey(char key)
{
  Keyboard.write(key);
}

void pressKey(char modifier, char key)
{
  Keyboard.press(modifier);
  Keyboard.write(key);
  Keyboard.release(modifier);
}

void click(signed char x, signed char y, char button)
{
  Mouse.move(x, y);
  Mouse.click(button);
}

void loop()
{
  static const char PREAMBLE = 0xDC;
  static const uint8_t BUFFER_SIZE = 5;
  enum
  {
    KEYBOARD_COMMAND = 0x1,
    KEYBOARD_MODIFIER_COMMAND = 0x2,
    MOUSE_COMMAND = 0x3
  };
  
  if (Serial.available() > 0)
  {
    char buffer[BUFFER_SIZE] = {0};
    uint8_t readBytes = Serial.readBytes(buffer, BUFFER_SIZE);
    
    if (readBytes != BUFFER_SIZE)
      return;

    if (buffer[0] != PREAMBLE)
      return;

    switch(buffer[1])
    {
      case KEYBOARD_COMMAND:
        pressKey(buffer[3]);
        break;

      case KEYBOARD_MODIFIER_COMMAND:
        pressKey(buffer[2], buffer[3]);
        break;

      case MOUSE_COMMAND:
        click(buffer[2], buffer[3], buffer[4]);
        break;
    }
  }  
}