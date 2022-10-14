#include "main.h"
#include "ports.h"

using namespace pros;

class DriveSide
{
    private:
    Motor front;
    Motor back;

    public:
    DriveSide(void);

    DriveSide(Motor _front, Motor _back) : front(_front), back(_back) {}

    void move_relative(double position, int velocity)
    {
        front.move_relative(position, velocity);
        back.move_relative(position, velocity);
    }

    void move_voltage(int voltage)
    {
        front.move_voltage(voltage);
        back.move_voltage(voltage);
    }

    void move(int power)
    {
        front.move(power);
        back.move(power);
    }

    void set_velocity(const std::int32_t velocity)
    {
        front.move_velocity(velocity);
        back.move_velocity(velocity);
    }

    void brake()
    {
        front.brake();
        back.brake();
    }
};

class Drivetrain
{
    private:
    Motor drive_fl;
    Motor drive_fr;
    Motor drive_bl;
    Motor drive_br;

    public:
    Drivetrain();
    DriveSide left = DriveSide(drive_fl, drive_bl);
    DriveSide right = DriveSide(drive_fr, drive_br);

    void move_relative(float target, float vel);
    void move_voltage(int voltage);
    void move(int voltage);
    void move_relative(double position, int velocity);
    void brake();
};
