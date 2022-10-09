#include "main.h"
#include "ports.h"

using namespace pros;

void flywheel_on_fn(void *fly_power_p)
{
    Motor fly1(FLY1_PORT);
    Motor fly2(FLY2_PORT);

    /* Diagnostic Variables */
    int start_time = millis();
    double elapsed_time = 0;
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

    // Get the actual speed
    // (Casts the generic pointer to an int pointer that can be dereferenced, and then dereferences that pointer)
    int speed = *((int *)fly_power_p);

    // Cap the value
    speed = (speed <= 12000) ? speed : 12000;

    // Ramp up
    for (int i = fly1.get_voltage(); i <= speed; i += 50)
    {
        fly1.move_voltage(i);
        fly2.move_voltage(i);

        /* DIAGNOSTICS */
        elapsed_time = (millis() - start_time) / 1000.0;

        real_vel = (fly1.get_actual_velocity() - fly2.get_actual_velocity()) / 2;
        real_current = fly1.get_current_draw();
        torque_vel_power = fly1.get_power();
        amps_volts_power = (real_voltage * real_current) / 1000000.0;
        real_temp = fly1.get_temperature();
        real_torque = fly1.get_torque();

        printf("%.2f, %.3f, %i, %i, %.3f, %.3f, %.3f, %.3f\n", elapsed_time, real_vel, real_voltage, real_current,
               torque_vel_power, amps_volts_power, real_temp, real_torque);
        delay(10);
    }
}

void flywheel_off_fn()
{
    Motor fly1(FLY1_PORT);
    Motor fly2(FLY2_PORT);

    // Ramp down from current speed
    for (int i = fly1.get_voltage(); i >= 0; i -= 50)
    {
        fly1.move_voltage(i);
        fly2.move_voltage(i);
        delay(10);
    }
}

void index_disc()
{
    ADIDigitalOut indexer(INDEX_PORT);
    Motor intake(INTAKE_PORT);

    intake.move(127);
    indexer.set_value(false);
    delay(250);
    indexer.set_value(true);
    delay(100);
    intake.brake();
}
