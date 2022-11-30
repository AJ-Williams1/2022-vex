#include "main.h"
#include "ports.h"
#include "setup_macros.h"

using namespace okapi;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller ctrl;

    setup_chassis;

    // setup_flywheel;

    Motor intake(-INTAKE_PORT);

    Motor colorwheel(COLORWHEEL_PORT);

    pros::ADIDigitalOut indexer(INDEX_PORT);

    // Joystick variables
    double joy_l_x = 0;
    double joy_l_y = 0;
    double joy_r_x = 0;
    double joy_r_y = 0;

    double l_drive_speed = 0;
    double r_drive_speed = 0;

    bool drive_disabled = false;

    pros::Task flywheel_on(flyCalc, nullptr);
    

    indexer.set_value(true);

    while (true)
    {
        if (ctrl.operator[](ControllerDigital::L1).changedToPressed())
            //flywheel_speed = 80;
            flywheel_speed = 40;
        else if (ctrl.operator[](ControllerDigital::L2).changedToPressed())
            //flywheel_speed = 0;
            flywheel_speed = 0;
        else if (ctrl.operator[](ControllerDigital::up).changedToPressed())
            //flywheel_speed = 5;
            flywheel_speed += 2.5;
        else if (ctrl.operator[](ControllerDigital::down).changedToPressed())
            //flywheel_speed = -5;
            flywheel_speed -= 2.5;
        else if (ctrl.operator[](ControllerDigital::right).changedToPressed())
            //flywheel_speed = +20;
            flywheel_speed += 10;
        else if (ctrl.operator[](ControllerDigital::left).changedToPressed())
            //flywheel_speed = -20;
            flywheel_speed -= 10;

        // if (flyCtrl->getTarget() != flywheel_speed)
        // flyCtrl->setTarget(flywheel_speed);

        // printf("%f\n", flyCtrl->getProcessValue());

        // Indexer
        if (ctrl.operator[](ControllerDigital::A).changedToPressed())
            index_disc();

        // Intake
        if (ctrl.getDigital(ControllerDigital::R1))
            intake.moveVoltage(12000);
        else if (ctrl.getDigital(ControllerDigital::R2))
            intake.moveVoltage(-12000);
        else
            intake.moveVoltage(0);

        // Color Wheel
        if (ctrl.getDigital(ControllerDigital::Y))
            colorwheel.moveVelocity(200);
        else
            colorwheel.moveVoltage(0);

        // Expansion
        if (ctrl.getDigital(ControllerDigital::R1) && ctrl.getDigital(ControllerDigital::R2) &&
            ctrl.getDigital(ControllerDigital::L1) && ctrl.getDigital(ControllerDigital::L2))
        {
            colorwheel.moveVoltage(-12000);
            pros::delay(100);
            colorwheel.moveVoltage(0);
        }

        // Drive
        /*
        if (ctrl.operator[](ControllerDigital::X))
        {
            drive_disabled = !drive_disabled;
        }

        if (drive_disabled)
            continue;
        */

        joy_l_y = ctrl.getAnalog(ControllerAnalog::leftY);
        joy_r_x = ctrl.getAnalog(ControllerAnalog::rightX);

        // Arcade style
        l_drive_speed = (joy_l_y + joy_r_x);
        r_drive_speed = (joy_l_y - joy_r_x);

        // Cap the value
        if (l_drive_speed > 1)
            l_drive_speed = 1;
        if (l_drive_speed < -1)
            l_drive_speed = -1;
        if (r_drive_speed > 1)
            r_drive_speed = 1;
        if (r_drive_speed < -1)
            r_drive_speed = -1;

        // If intake is running, drive forward, otherwise backwards
        if (!ctrl.getDigital(ControllerDigital::R1) && !ctrl.getDigital(ControllerDigital::R2))
        {
            r_drive_speed = r_drive_speed + l_drive_speed;
            l_drive_speed = (r_drive_speed - l_drive_speed);
            r_drive_speed = (r_drive_speed - l_drive_speed);
            l_drive_speed *= -1;
            r_drive_speed *= -1;
        }

        // Move motors
        drive->getModel()->left(l_drive_speed);
        drive->getModel()->right(r_drive_speed);

        setFlyAuto(flywheel_speed);
        pros::delay(10);
    }
}
