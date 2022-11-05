#include "main.h"
#include "ports.h"
#include "Drivetrain_main.h"
#include "autoSelect/selection.h"

using namespace pros;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize()
{
    lcd::initialize();
    // selector::init();

    // Flywheels
    Motor fly1(FLY1_PORT, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_COUNTS);
    Motor fly2(FLY2_PORT, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_COUNTS);

    // Drive motors
    Motor drive_fr(WHEEL_FR_PORT, E_MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
    Motor drive_fl(WHEEL_FL_PORT, E_MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
    Motor drive_br(WHEEL_BR_PORT, E_MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
    Motor drive_bl(WHEEL_BL_PORT, E_MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);

    // Intake motor
    Motor intake(INTAKE_PORT, E_MOTOR_GEARSET_36, true, MOTOR_ENCODER_ROTATIONS);

    // Colorwheel motor
    Motor colorwheel(COLORWHEEL_PORT, E_MOTOR_GEARSET_36, true, MOTOR_ENCODER_ROTATIONS);

    ADIDigitalOut indexer(INDEX_PORT);
    indexer.set_value(true);

    ADIDigitalOut expand(EXPAND_PORT);

    Vision vis(VISION_PORT, E_VISION_ZERO_CENTER);
    vis.set_wifi_mode(1);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
}
