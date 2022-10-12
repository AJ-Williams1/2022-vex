#include "main.h"
#include "ports.h"

#define WHEEL_DIAMETER 4.0

using namespace pros;

class drive
{
    drive(Motor _drive_fr, Motor _drive_fl, Motor _drive_br, Motor _drive_bl)
    {
        Motor drive_fr = _drive_fr;
        Motor drive_fl = _drive_fl;
        Motor drive_br = _drive_br;
        Motor drive_bl = _drive_bl;
    }

}

// Functions analagous to motor functions like move(), move_voltage, etc. but for all drive motors at once
void drive_move_relative(float target, float vel)
{
    Motor drive_fr(WHEEL_FR_PORT);
    Motor drive_fl(WHEEL_FL_PORT);
    Motor drive_br(WHEEL_BR_PORT);
    Motor drive_bl(WHEEL_BL_PORT);

    drive_fr.move_relative(target, vel);
    drive_fl.move_relative(target, vel);
    drive_br.move_relative(target, vel);
    drive_bl.move_relative(target, vel);
}

void drive_move_voltage(int voltage)
{
    Motor drive_fr(WHEEL_FR_PORT);
    Motor drive_fl(WHEEL_FL_PORT);
    Motor drive_br(WHEEL_BR_PORT);
    Motor drive_bl(WHEEL_BL_PORT);

    drive_fr.move_voltage(voltage);
    drive_fl.move_voltage(voltage);
    drive_br.move_voltage(voltage);
    drive_bl.move_voltage(voltage);
}

void drive_move(int voltage)
{
    Motor drive_fr(WHEEL_FR_PORT);
    Motor drive_fl(WHEEL_FL_PORT);
    Motor drive_br(WHEEL_BR_PORT);
    Motor drive_bl(WHEEL_BL_PORT);

    drive_fr.move(voltage);
    drive_fl.move(voltage);
    drive_br.move(voltage);
    drive_bl.move(voltage);
}

// Autonomous helper functions
void drive_dist(float distance, float vel)
{
    float rotations = distance / (WHEEL_DIAMETER * M_PI);
    drive_move_relative(rotations, vel);
}
