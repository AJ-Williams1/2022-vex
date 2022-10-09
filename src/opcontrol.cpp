#include "flywheel.h"
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

    Motor fly1(FLY1_PORT);
    Motor fly2(FLY2_PORT);

    ADIDigitalOut indexer(INDEX_PORT);

    // Joystick variables
    int32_t joy_l_x = 0;
    int32_t joy_l_y = 0;
    int32_t joy_r_x = 0;
    int32_t joy_r_y = 0;

    int32_t l_drive_speed = 0;
    int32_t r_drive_speed = 0;

    // Start and end both flywheel tasks, so that I can check if they're running later
    Task flywheel_on(flywheel_on_fn, (void *)12000, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
                     "Turn Flywheel On");
    Task flywheel_off(flywheel_off_fn);

    flywheel_on.remove();
    flywheel_off.remove();

    indexer.set_value(true);

    while (true)
    {

        // Flywheel
        if (ctrl.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        {
            // Run the flywheel_on task only if it's not already running, and kill the flywheel_off task
            if (flywheel_on.get_state() != E_TASK_STATE_RUNNING)
            {
                // All this pointer nonsense is unfortunately necessary
                int *fly_power_p = (int *)malloc(sizeof(int));
                *fly_power_p = 12000;
                Task flywheel_on(flywheel_on_fn, (void *)fly_power_p, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
                                 "Turn Flywheel On");
                if (flywheel_off.get_state() == E_TASK_STATE_RUNNING)
                    flywheel_off.remove();
            }
        }
        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_L2))
        {
            // Same as above, but for flywheel_off
            if (flywheel_off.get_state() != E_TASK_STATE_RUNNING)
            {
                Task flywheel_off(flywheel_off_fn);

                if (flywheel_on.get_state() == E_TASK_STATE_RUNNING)
                    flywheel_on.remove();

                flywheel_off.notify();
            }
        }

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

        // If intake is running, drive forward, otherwise backwards
        if (!ctrl.get_digital(E_CONTROLLER_DIGITAL_R1))
        {
            l_drive_speed *= -1;
            r_drive_speed *= -1;
        }

        // Move motors
        drive_fr.move(r_drive_speed);
        drive_br.move(r_drive_speed);
        drive_fl.move(l_drive_speed);
        drive_bl.move(l_drive_speed);

        delay(10);
    }
}
