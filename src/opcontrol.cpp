#include "main.h"
#include "ports.h"

using namespace pros;

/* SETUP DEVICES */
Controller control(E_CONTROLLER_MASTER);

Motor fly1(FLY1_PORT);
Motor fly2(FLY2_PORT);

Motor drive_fr(WHEEL_FR_PORT);
Motor drive_fl(WHEEL_FL_PORT);
Motor drive_br(WHEEL_BR_PORT);
Motor drive_bl(WHEEL_BL_PORT);

Motor intake(INTAKE_MOTOR);

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    // Joystick variables
    int32_t joy_l_x = 0;
    int32_t joy_l_y = 0;
    int32_t joy_r_x = 0;
    int32_t joy_r_y = 0;

    int32_t l_drive_speed = 0;
    int32_t r_drive_speed = 0;

    while (true)
    {
        // Intake
        if (control.get_digital(E_CONTROLLER_DIGITAL_A))
            intake.move(127);
        else
            intake.brake();

        // Drive
        joy_l_x = control.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
        joy_l_y = control.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
        joy_r_x = control.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
        joy_r_y = control.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);

        // Arcade style
        l_drive_speed = joy_l_y + joy_r_x;
        r_drive_speed = joy_l_y - joy_r_x;

        // Move motors
        drive_fr.move(r_drive_speed);
        drive_br.move(r_drive_speed);
        drive_fl.move(l_drive_speed);
        drive_bl.move(l_drive_speed);

        delay(20);
    }
}
