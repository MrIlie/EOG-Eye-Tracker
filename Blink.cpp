#include "Arduino.h"

#define SAMPLES 200000

unsigned int blinks = 0;          //contorizeaza numarul de clipiri


void EyeBlink( void )
{
  blinks++;
}

int Blink_Frequency( void )
{
  return blinks / SAMPLES;
}

