#include "main.h"
#include "ports.h"

using namespace pros;

float flywheel_speed;

void flywheel_on_fn()
{
    printf("flywheel_on_fn called\n");
    pros::Motor fly1(FLY1_PORT);
    pros::Motor fly2(FLY2_PORT);
    Controller ctrl(E_CONTROLLER_MASTER);

    float target = 0;

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
    // printf(
    //"time (ms),target (RPM),velocity (RPM),voltage (mV),current (mA),torque*vel power (W),current*volts power (W),"
    //"temperature (deg C),torque (Nm)\n");

    // Get the actual speed
    // (Casts the generic pointer to an int pointer that can be dereferenced, and then dereferences that pointer)

    double fly1_error = 0;
    double fly2_error = 0;

    double fly1_vel = fly1.get_actual_velocity();
    double fly2_vel = fly2.get_actual_velocity();


    double fly1_vel_sum = 0;
    double fly2_vel_sum = 0;

    int fly1_voltage = 0;
    int fly2_voltage = 0;
    int ticks = 0;

    // float kP = 0.0018;
    float kP = 0.006;

    // printf("ticks, fly1_vel, fly2_vel\n");

    while (true)
    {
        ctrl.set_text(0, 0,
                      std::to_string((int)round(flywheel_speed)) + "  " +
                          std::to_string((int)round((fly1_vel + fly2_vel) / 2)) + "     ");

        // Get & cap target value
        target = (flywheel_speed <= 200) ? flywheel_speed : 200;

        // printf("fly1_vel: %.3f,  fly1_volts: %i, fly1_realvolts: %i\nfly2_vel: %.3f, fly2_volts: %i, fly2_realvolts:
        // "
        // "%i\n\n",
        // fly1_vel, fly1_voltage, fly1.get_voltage(), fly2_vel, fly2_voltage, fly2.get_voltage());

        // ticks++;
        // printf("%i,%.3f,%.3f\n", ticks, fly1_vel, fly2_vel);

        fly1_vel = fly1.get_actual_velocity();
        fly2_vel = fly2.get_actual_velocity();

        fly1_vel_sum += fly1_vel;
        fly2_vel_sum += fly2_vel;

        fly1_error = target - fly1_vel;
        fly2_error = target - fly2_vel;
        

        fly1_voltage += round((fly1_error * 200.0 * kP));
        fly2_voltage += round((fly2_error * 200.0 * kP));

        fly1.move_voltage(fly1_voltage);
        fly2.move_voltage(fly2_voltage);

        delay(50);
    }
    /*
    // Diagnostics
    elapsed_time = (millis() - start_time) / 1000.0;

    real_vel = (fly1.get_actual_velocity() - fly2.get_actual_velocity()) / 2;
    real_current = fly1.get_current_draw();
    torque_vel_power = fly1.get_power();
    amps_volts_power = (real_voltage * real_current) / 1000000.0;
    real_temp = fly1.get_temperature();
    real_torque = fly1.get_torque();

    // printf("%.2f, %.3f, %i, %i, %.3f, %.3f, %.3f, %.3f\n", elapsed_time, real_vel, real_voltage, real_current,
           //torque_vel_power, amps_volts_power, real_temp, real_torque);
    */
}
