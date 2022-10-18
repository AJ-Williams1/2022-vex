#include "autoSelect/selection.h"
#include "main.h"
#include "ports.h"

using namespace okapi;

// Constants
#define WHEEL_DIAMETER 4.0
#define WHEELBASE_WIDTH 11.5
#define WHEEL_TO_EDGE 2

// Types
typedef enum circle_radius_type_e
{
    INNER = -1,
    OUTER = 1,
} circle_radius_type_e;

typedef enum front_side_e
{
    FLY = 1,
    INTAKE = -1,
} front_side_e;

typedef enum circle_direction_e
{
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1,
} circle_direction_e;

// Function declarations
void color_quick_spin();
void drive_dist(float distance, float vel);
void drive_circle(circle_radius_type_e radius_type, float radius, float outer_speed, float degrees,
                  circle_direction_e direction, front_side_e front);
void drive_time(int time, float vel);

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
    drive_circle(INNER, 4, 80, 190, CLOCKWISE, FLY);
    delay(4000);
    drive_time(100, -80);
    color_quick_spin();
}

// Helper functions

void color_quick_spin()
{
    Motor colorwheel(COLORWHEEL_PORT);

    colorwheel.move(127);
    delay(250);
    colorwheel.brake();
}

void drive_time(int time, float vel)
{
    Drivetrain drive;

    drive.move(vel);
    delay(time);
    drive.brake();
}

void drive_dist(float distance, float vel)
{
    Drivetrain drive;

    float rotations = distance / (WHEEL_DIAMETER * M_PI);
    drive.move_relative(rotations, vel);
}

void drive_circle(circle_radius_type_e radius_type, float radius, float outer_speed, float degrees,
                  circle_direction_e direction, front_side_e front)
{
    Drivetrain drive;

    // Get both actual radii, with respect to wheels, not edge of robot
    float inner_radius = 0;
    float outer_radius = 0;

    if (radius_type == INNER)
    {
        inner_radius = radius + WHEEL_TO_EDGE;
        outer_radius = inner_radius + WHEELBASE_WIDTH;
    }

    else if (radius_type == OUTER)
    {
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

    // Reverse direction if you want to drive towards the flywheel
    if (front == FLY)
    {
        inner_rotations = -inner_rotations;
        outer_rotations = -outer_rotations;
    }

    // Select which side is inner or outer based on direction of rotation
    if (direction == CLOCKWISE)
    {
        drive.right.move_relative(inner_rotations, inner_speed);
        drive.left.move_relative(outer_rotations, outer_speed);
    }

    else if (direction == COUNTERCLOCKWISE)
    {
        drive.left.move_relative(inner_rotations, inner_speed);
        drive.right.move_relative(outer_rotations, outer_speed);
    }
}
