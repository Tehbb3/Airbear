#include "globals.h"
#include "config.h"

Preferences config;

void initConfig()
{
  Serial.println("Initialising config");

  config.begin("air-bear", false);

  if(config.getUChar("connection_type", 0) == 0) { config.putUChar("connection_type", CONNECTION_TYPE_DASH); } //Default to WIFI mode

  Serial.println("Config Initialised");
  Serial.print("Connection type: ");
  Serial.println(config.getUChar("connection_type"));
}