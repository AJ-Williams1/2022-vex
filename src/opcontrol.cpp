#include "flywheel.h"
#include "main.h"
#include "ports.h"
#include "Drivetrain_main.h"

using namespace pros;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller ctrl(E_CONTROLLER_MASTER);

    Drivetrain drive;

    Motor intake(INTAKE_PORT);

    Motor fly1(FLY1_PORT);
    Motor fly2(FLY2_PORT);

    Motor colorwheel(COLORWHEEL_PORT);

    ADIDigitalOut indexer(INDEX_PORT);

    // Joystick variables
    int32_t joy_l_x = 0;
    int32_t joy_l_y = 0;
    int32_t joy_r_x = 0;
    int32_t joy_r_y = 0;

    int32_t l_drive_speed = 0;
    int32_t r_drive_speed = 0;

    bool drive_disabled = false;

    // Start and end both flywheel tasks, so that I can check if they're running later
    Task flywheel_on(flywheel_on_fn);

    indexer.set_value(true);

    while (true)
    {
        // Flywheel
        if (ctrl.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
            flywheel_speed = 100;
        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_L2))
            flywheel_speed = 0;

        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_UP))
            flywheel_speed += 5;
        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN))
            flywheel_speed -= 5;

        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT))
            flywheel_speed += 20;
        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT))
            flywheel_speed -= 20;

        // Indexer
        if (ctrl.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
            index_disc();

        // Intake
        if (ctrl.get_digital(E_CONTROLLER_DIGITAL_R1))
            intake.move(127);
        else if (ctrl.get_digital(E_CONTROLLER_DIGITAL_R2))
            intake.move(-127);
        else
            intake.brake();

        // Color Wheel
        if (ctrl.get_digital(E_CONTROLLER_DIGITAL_Y))
            colorwheel.move(127);
        else
            colorwheel.brake();

        // Drive
        if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_X))
        {
            drive_disabled = !drive_disabled;
        }

        if (drive_disabled)
            continue;

        joy_l_x = ctrl.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
        joy_l_y = ctrl.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
        joy_r_x = ctrl.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
        joy_r_y = ctrl.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);

        // Arcade style
        l_drive_speed = joy_l_y + joy_r_x;
        r_drive_speed = joy_l_y - joy_r_x;

        // Cap the value
        if (joy_l_y + joy_r_x > 127)
            l_drive_speed = 127;
        if (joy_l_y + joy_r_x < -127)
            l_drive_speed = -127;
        if (joy_l_y - joy_r_x > 127)
            r_drive_speed = 127;
        if (joy_l_y - joy_r_x < -127)
            r_drive_speed = -127;

        // If intake is running, drive forward, otherwise backwards
        if (!ctrl.get_digital(E_CONTROLLER_DIGITAL_R1))
        {
            r_drive_speed = r_drive_speed + l_drive_speed;
            l_drive_speed = (r_drive_speed - l_drive_speed);
            r_drive_speed = (r_drive_speed - l_drive_speed);
            l_drive_speed *= -1;
            r_drive_speed *= -1;
        }

        // Move motors
        drive.move(r_drive_speed);
        delay(20);
    }
}
