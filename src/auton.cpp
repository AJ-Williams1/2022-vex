#include "main.h"
#include "ports.h"
#include "Drivetrain_main.h"
#include "autoSelect/selection.h"

using namespace pros;

// Constants
#define WHEEL_DIAMETER 4.0
#define WHEELBASE_WIDTH 11.5
#define WHEEL_TO_EDGE 2

// Types
typedef enum circle_radius_type_e {
    INNER = -1,
    OUTER = 1,
} circle_radius_type_e;

typedef enum circle_direction_e {
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1,
} circle_direction_e;

// Function declarations
void drive_dist(float distance, float vel);
void drive_circle(circle_radius_type_e radius_type, float radius, float outer_speed, float radians, circle_direction_e direction);

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

void auton()
{
    drive_circle(INNER, 24, 80, 360, CLOCKWISE);
}

// Helper functions
void drive_dist(float distance, float vel)
{
    Drivetrain drive;

    float rotations = distance / (WHEEL_DIAMETER * M_PI);
    drive.move_relative(rotations, vel);
}

void drive_circle(circle_radius_type_e radius_type, float radius, float outer_speed, float degrees, circle_direction_e direction)
{
    Drivetrain drive;

    // Get both actual radii, with respect to wheels, not edge of robot
    float inner_radius = 0;
    float outer_radius = 0;

    switch (radius_type)
    {
    case INNER:
        inner_radius = radius + WHEEL_TO_EDGE;
        outer_radius = inner_radius + WHEELBASE_WIDTH;

    case OUTER:
        outer_radius = radius - WHEEL_TO_EDGE;
        inner_radius = outer_radius - WHEELBASE_WIDTH;
    }

    // Find the velocities, based on the formula for driving in circles:
    // (V_outer / V_inner) = (R_outer / R_inner)
    float inner_speed = outer_speed * (inner_radius / outer_radius);

    // Convert degrees to radians and get arc length
    float inner_arc_length = (degrees * M_PI / 180) * inner_radius;
    float outer_arc_length = (degrees * M_PI / 180) * outer_radius;

    float inner_rotations = inner_arc_length / (WHEEL_DIAMETER * M_PI);
    float outer_rotations = outer_arc_length / (WHEEL_DIAMETER * M_PI);

    switch(direction)
    {
        case CLOCKWISE:
            drive.right.move_relative(inner_rotations, inner_speed);
            drive.left.move_relative(outer_rotations, outer_speed);

        case COUNTERCLOCKWISE:
            drive.left.move_relative(inner_rotations, inner_speed);
            drive.right.move_relative(outer_rotations, outer_speed);
    }
}
