#include "main.h"
#include "ports.h"

using namespace okapi;

void index_disc()
{
    pros::ADIDigitalOut indexer(INDEX_PORT);
    Motor intake(INTAKE_PORT);

    intake.moveVoltage(12000);
    indexer.set_value(false); // Fire
    pros::delay(250);
    indexer.set_value(true); // Retract indexer
    pros::delay(500);        // Let intake run a little longer to make sure next disc falls down
    intake.moveVoltage(0);
}

void aim_horiz()
{
    pros::lcd::initialize();
    pros::Vision vis(VISION_PORT, pros::E_VISION_ZERO_CENTER);
    pros::vision_signature_s_t red_goal =
        pros::Vision::signature_from_utility(1, 4475, 8221, 6348, -967, 651, -158, 1.6, 0);

    while (true)
    {
        pros::vision_object_s_t bottom = vis.get_by_sig(0, 1);
        pros::delay(20);
    }
}
