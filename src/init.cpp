#include "main.h"
#include "ports.h"
#include "autoSelect/selection.h"

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
    Motor fly1(FLY1_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::rotations,
               Logger::getDefaultLogger());
    Motor fly2(FLY2_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::rotations,
               Logger::getDefaultLogger());

    // Drive motors
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
