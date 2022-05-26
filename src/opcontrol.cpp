#include "main.h"
#include <chrono>

using namespace pros;
using namespace std::chrono;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller master(E_CONTROLLER_MASTER);
    Motor fly1(10);

    /* Time variables */
    steady_clock::time_point start_time = steady_clock::now();
    milliseconds duration_from_start; // The duration object for the time elapsed
    long long ms_from_start = 0;      // The number of milliseconds elapsed

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
        /* Time */
        duration_from_start = duration_cast<milliseconds>(steady_clock::now() - start_time);
        ms_from_start = duration_from_start.count();

        /* Motor stats */
        real_vel = fly1.get_actual_velocity();
        real_voltage = fly1.get_voltage();
        real_current = fly1.get_current_draw();
        real_power = fly1.get_power();
        real_temp = fly1.get_temperature();
        real_torque = fly1.get_torque();

        printf("%lld, %f, %i, %i, %f, %f, %f\n", ms_from_start, real_vel, real_voltage, real_current, real_power,
               real_temp, real_torque);

        /* Actual stuff */
        delay(20);
    }
}
