#include "main.h"
#include "ports.h"

using namespace okapi;

/* In its own task when using FMS or Comp Switch, otherwise runs after init */
void opcontrol()
{
    /* SETUP DEVICES */
    Controller ctrl;

    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors({WHEEL_FL_PORT, WHEEL_BL_PORT}, {WHEEL_FR_PORT, WHEEL_BR_PORT})
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
            .withGains({0.001, 0, 0.0001}, // Distance controller gains
                       {0.001, 0, 0.0001}, // Turn controller gains
                       {0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
                       )
            .withOdometry()
            .buildOdometry();

    Motor intake(INTAKE_PORT);

    Motor fly1(FLY1_PORT);
    Motor fly2(FLY2_PORT);

    Motor colorwheel(COLORWHEEL_PORT);

    pros::ADIDigitalOut indexer(INDEX_PORT);

    // Joystick variables
    int32_t joy_l_x = 0;
    int32_t joy_l_y = 0;
    int32_t joy_r_x = 0;
    int32_t joy_r_y = 0;

    int32_t l_drive_speed = 0;
    int32_t r_drive_speed = 0;

    bool drive_disabled = false;

    // Start and end both flywheel tasks, so that I can check if they're running later
    pros::Task flywheel_on(flywheel_on_fn);

    indexer.set_value(true);

    // autonomous();

    // delay(10000000);

    while (true)
    {
        if (ctrl.operator[](ControllerDigital::L1).changedToPressed())
            flywheel_speed = 100;
        else if (ctrl.operator[](ControllerDigital::L2).changedToPressed())
            flywheel_speed = 0;
        else if (ctrl.operator[](ControllerDigital::up).changedToPressed())
            flywheel_speed += 5;
        else if (ctrl.operator[](ControllerDigital::down).changedToPressed())
            flywheel_speed -= 5;

        else if (ctrl.operator[](ControllerDigital::right).changedToPressed())
            flywheel_speed += 20;
        else if (ctrl.operator[](ControllerDigital::left).changedToPressed())
            flywheel_speed -= 20;

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
            colorwheel.moveVoltage(12000);
        else
            colorwheel.moveVoltage(0);

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
        l_drive_speed = 200 * (joy_l_y + joy_r_x);
        r_drive_speed = 200 * (joy_l_y - joy_r_x);

        // Cap the value
        if (joy_l_y + joy_r_x > 200)
            l_drive_speed = 200;
        if (joy_l_y + joy_r_x < -200)
            l_drive_speed = -200;
        if (joy_l_y - joy_r_x > 200)
            r_drive_speed = 200;
        if (joy_l_y - joy_r_x < -200)
            r_drive_speed = -200;

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

        pros::delay(10);
    }
}
