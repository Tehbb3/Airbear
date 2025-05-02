#include "logger.h"
#include "config.h"

void debugMsg(String msg, uint8_t pri)
{
  if(pri >= config.getUChar("debugLevel"))
  {
    if(config.getBool("debugSerial"))
    {
      Serial.println(msg);
    }
    if(config.getBool("debugWeb"))
    {
      Serial.println(msg);
    }
  }
}