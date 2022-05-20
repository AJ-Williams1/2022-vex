#include "main.h"

using namespace pros;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    Controller master(E_CONTROLLER_MASTER);

    while (true)
    {
        delay(20);
    }
}
