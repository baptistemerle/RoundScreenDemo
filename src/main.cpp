#include <Arduino.h>

#include "Core/main_app.h"

MainApp app;

void setup()
{
  app.setup();
}

void loop()
{
  app.loop();
}