#include "main.h"
#include <time.h>

using namespace pros;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller master(E_CONTROLLER_MASTER);
    Motor fly1(10);

    /* Time variables */
    time_t start_time = time(NULL);
    double elapsed_time = 0;

    /* Diagnostic Variables */
    double real_vel = 0;
    int32_t real_voltage = 0;
    int32_t real_current = 0;
    double real_power = 0;
    double real_temp = 0;
    double real_torque = 0;

    /* Print headers for daignostic variables */
    printf("time (ms), velocity (RPM), voltage (mV), current(mA), power (W), temperature (deg C), torque (Nm)\n");

    fly1.move(100);

    while (true)
    {
        /* Diagnostics */
        elapsed_time = 1000 * difftime(time(NULL), start_time);
        real_vel = fly1.get_actual_velocity();
        real_voltage = fly1.get_voltage();
        real_current = fly1.get_current_draw();
        real_power = fly1.get_power();
        real_temp = fly1.get_temperature();
        real_torque = fly1.get_torque();

        printf("%f, %f, %i, %i, %f, %f, %f", elapsed_time, real_vel, real_voltage, real_current, real_power, real_temp,
               real_torque);

        /* Actual stuff */

        delay(10);
    }
}
