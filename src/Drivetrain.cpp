#include "main.h"
#include "ports.h"
#include "Drivetrain_main.h"

using namespace pros;


    Drivetrain::Drivetrain():drive_fl(WHEEL_FL_PORT), drive_fr(WHEEL_FR_PORT), drive_bl(WHEEL_BL_PORT), drive_br(WHEEL_BR_PORT) {}

    void Drivetrain::move_relative(float target, float vel)
    {
        drive_fr.move_relative(target, vel);
        drive_fl.move_relative(target, vel);
        drive_br.move_relative(target, vel);
        drive_bl.move_relative(target, vel);
    }

    void Drivetrain::move_voltage(int voltage)
    {
        drive_fr.move_voltage(voltage);
        drive_fl.move_voltage(voltage);
        drive_br.move_voltage(voltage);
        drive_bl.move_voltage(voltage);
    }

    void Drivetrain::move(int voltage)
    {
        drive_fr.move(voltage);
        drive_fl.move(voltage);
        drive_br.move(voltage);
        drive_bl.move(voltage);
    }

    void Drivetrain::move_relative(double position, int velocity)
    {
        drive_fr.move_relative(position, velocity);
        drive_fl.move_relative(position, velocity);
        drive_br.move_relative(position, velocity);
        drive_bl.move_relative(position, velocity);
    }

    void Drivetrain::brake()
    {
        drive_fr.brake();
        drive_fl.brake();
        drive_br.brake();
        drive_bl.brake();
    }

