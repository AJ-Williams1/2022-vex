#include "main.h"

using namespace pros;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller master(E_CONTROLLER_MASTER);
    Motor fly1(13);
    Motor fly2(14);

    Motor drive_fr(WHEEL_FR_PORT);
    Motor drive_fl(WHEEL_FL_PORT);
    Motor drive_br(WHEEL_BR_PORT);
    Motor drive_bl(WHEEL_BL_PORT);

    Motor intake(INTAKE_MOTOR);

    /* Diagnostic Variables */
    double real_vel = 0;
    int32_t real_voltage = 0;
    int32_t real_current = 0;
    double torque_vel_power = 0;
    double amps_volts_power = 0;
    double real_temp = 0;
    double real_torque = 0;

    /* Print headers for daignostic variables */
    printf("time (ms),velocity (RPM),voltage (mV),current (mA),torque*vel power (W),current*volts power (W),"
           "temperature (deg C),torque (Nm)\n");

    fly1.move(-127);
    fly2.move(-127);
    // 7 + 7/16 in compression

    /* Time variables */
    int start_time = millis();
    double elapsed_time = 0;

    while (true)
    {
        /* Time */
        elapsed_time = (millis() - start_time) / 1000.0;

        /* Motor stats */
        real_vel = fly1.get_actual_velocity();
        real_voltage = fly1.get_voltage();
        real_current = fly1.get_current_draw();
        torque_vel_power = fly1.get_power();
        amps_volts_power = (real_voltage * real_current) / 1000000.0;
        real_temp = fly1.get_temperature();
        real_torque = fly1.get_torque();

        printf("%.2f, %.3f, %i, %i, %.3f, %.3f, %.3f, %.3f\n", elapsed_time, real_vel, real_voltage, real_current,
               torque_vel_power, amps_volts_power, real_temp, real_torque);

        /* Actual stuff */
        delay(50);
    }
}
