#include "main.h"
#include "ports.h"

using namespace pros;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller ctrl(E_CONTROLLER_MASTER);

    Motor drive_fr(WHEEL_FR_PORT);
    Motor drive_fl(WHEEL_FL_PORT);
    Motor drive_br(WHEEL_BR_PORT);
    Motor drive_bl(WHEEL_BL_PORT);

    Motor intake(INTAKE_PORT);

    Motor fly(FLY_PORT);

    // Joystick variables
    int32_t joy_l_x = 0;
    int32_t joy_l_y = 0;
    int32_t joy_r_x = 0;
    int32_t joy_r_y = 0;

    int32_t l_drive_speed = 0;
    int32_t r_drive_speed = 0;

    while (true)
    {
        // Flywheel
        fly.move(127);

        // Intake
        if (ctrl.get_digital(E_CONTROLLER_DIGITAL_A))
            intake.move(127);
        else
            intake.brake();

        // Drive
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

        // Move motors
        drive_fr.move(r_drive_speed);
        drive_br.move(r_drive_speed);
        drive_fl.move(l_drive_speed);
        drive_bl.move(l_drive_speed);

        delay(20);
    }
}
