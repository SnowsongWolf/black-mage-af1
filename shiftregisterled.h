#define REGISTERS = 6

const int pinData = 8;
const int pinClock = 7;
const int pinLock = 6;

byte registerData[REGISTERS];

void clearRegisterData()
{
  for (int i = 0; i < REGISTERS; i++)
  {
    registerData[i] = 0;
  }
}

void setPin(int pin, boolean val)
{
  registerData[(int)(pin / 8)] &= (0xff - (byte)((int)(0.5 + pow(2,(pin % 8)))));
}

void writeRegisterData()
{
  digitalWrite(PinLock, HIGH);
  for (int i = REGISTERS - 1; i >= 0; i --)
  {
    shiftOut();
  }
}
