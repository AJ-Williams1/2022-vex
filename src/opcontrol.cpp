#include "main.h"
#include "ports.h"

using namespace pros;

void flywheel_on_fn(void *raw_speed)
{
    while (Task::notify_take(true, TIMEOUT_MAX))
        ;

    Motor fly(FLY_PORT);

    // Get the actual speed
    // (Casts the generic pointer to an int pointer that can be dereferenced, and then dereferences that pointer)
    int32_t speed = *(int *)raw_speed;

    // Cap the value
    speed = (speed <= 127) ? speed : 127;

    if (fly.is_stopped())
    {
        for (int i = 0; i <= speed; i++)
        {
            fly.move(i);
            delay(10);
        }
    }
}

void flywheel_off_fn()
{
    while (Task::notify_take(true, TIMEOUT_MAX))
        ;

    Motor fly(FLY_PORT);

    if (!fly.is_stopped())
    {
        for (int i = 127; i >= 0; i--)
        {
            fly.move(i);
            delay(10);
        }
    }
}

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

    int8_t btn_state = -1;

    Task flywheel_on(flywheel_on_fn, (void *)127, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Turn Flywheel On");
    Task flywheel_off(flywheel_off_fn);

    flywheel_on.remove();
    flywheel_off.remove();

    while (true)
    {
        // Flywheel
        if (ctrl.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        {
            if (flywheel_on.get_state() != E_TASK_STATE_RUNNING)
            {
                Task flywheel_on(flywheel_on_fn, (void *)127, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
                                 "Turn Flywheel On");
                flywheel_on.notify();
            }
        }
        else if (ctrl.get_digital_new_press(E_CONTROLLER_DIGITAL_X))
        {
            if (flywheel_off.get_state() != E_TASK_STATE_RUNNING)
            {
                Task flywheel_off(flywheel_off_fn);
                flywheel_off.notify();
            }
        }

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

        // Move motors
        drive_fr.move(r_drive_speed);
        drive_br.move(r_drive_speed);
        drive_fl.move(l_drive_speed);
        drive_bl.move(l_drive_speed);

        delay(10);
    }
}
