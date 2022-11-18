#include "autoSelect/selection.h"
#include "main.h"
#include "ports.h"
#include "setup_macros.h"

using namespace okapi;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize()
{
    Logger::setDefaultLogger(std::make_shared<Logger>(TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
                                                      "/ser/sout",            // Output to the PROS terminal
                                                      Logger::LogLevel::debug // Show errors and warnings
                                                      ));
    pros::lcd::initialize();

    // Flywheels
    /*
    Motor fly1(FLY1_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::rotations,
               Logger::getDefaultLogger());
    Motor fly2(FLY2_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::rotations,
               Logger::getDefaultLogger());
    */


    setup_chassis;

    setup_flywheel;

    // Intake motor
    Motor intake(INTAKE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::rotations,
                 Logger::getDefaultLogger());

    // Colorwheel motor
    Motor colorwheel(COLORWHEEL_PORT, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::rotations,
                     Logger::getDefaultLogger());

    pros::ADIDigitalOut indexer(INDEX_PORT);
    indexer.set_value(true);

    pros::Vision vis(VISION_PORT, pros::E_VISION_ZERO_CENTER);
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
