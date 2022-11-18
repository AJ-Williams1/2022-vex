#include "autoSelect/selection.h"
#include "main.h"
#include "ports.h"
#include "setup_macros.h"

using namespace okapi;

// Constants
#define WHEEL_DIAMETER 4.0
#define WHEELBASE_WIDTH 11.5
#define WHEEL_TO_EDGE 2

// Function declarations
void color_quick_spin();

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous()
{
    setup_chassis;
}

// Helper functions

void color_quick_spin()
{
    Motor colorwheel(COLORWHEEL_PORT);

    colorwheel.moveVoltage(12000);
    pros::delay(250);
    colorwheel.moveVoltage(0);
}
