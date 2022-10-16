#include "Drivetrain_main.h"
#include "main.h"
#include "ports.h"

using namespace pros;

void index_disc()
{
    ADIDigitalOut indexer(INDEX_PORT);
    Motor intake(INTAKE_PORT);

    intake.move(127);
    indexer.set_value(false); // Fire
    delay(250);
    indexer.set_value(true); // Retract indexer
    delay(500); // Let intake run a little longer to make sure next disc falls down
    intake.brake();
}

void aim_horiz()
{
    lcd::initialize();
    Vision vis(VISION_PORT, pros::E_VISION_ZERO_CENTER);
    vision_signature_s_t red_goal = Vision::signature_from_utility(1, 4475, 8221, 6348, -967, 651, -158, 1.6, 0);
    Drivetrain drive;

    float kP = 0.3;

    float left_speed = 0;
    float right_speed = 0;

    int error = 158;

    while (true)
    {
        vision_object_s_t bottom = vis.get_by_sig(0, 1);

        error = bottom.x_middle_coord;
        lcd::print(0, "error: %i", error);

        left_speed = kP * error;
        right_speed = kP * -1 * error;

        drive.left.move(left_speed);
        drive.right.move(right_speed);

        delay(20);
    }
}
