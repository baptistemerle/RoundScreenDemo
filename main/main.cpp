#include "Core/main_app.h"

MainApp app;

extern "C"
{
  void app_main(void);
}

void app_main(void)
{
  app.setup();

  while (1)
  {
    app.loop();
  }
}