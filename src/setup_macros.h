#include "main.h"
#include "ports.h"

using namespace okapi;

#define setup_flywheel                                                                                                 \
    std::shared_ptr<AsyncVelocityController<double, double>> flyCtrl =                                                 \
        AsyncVelControllerBuilder()                                                                                    \
            .withMotor({-FLY1_PORT, FLY2_PORT})                                                                        \
            .withGains({0.00005, 0.00000, 0.00062, 0})                                                                 \
            .withVelMath(VelMathFactory::createPtr(imev5GreenTPR))                                                     \
            .build();

#define setup_chassis                                                                                                  \
    std::shared_ptr<OdomChassisController> drive =                                                                         \
        ChassisControllerBuilder()                                                                                     \
            .withMotors({WHEEL_FL_PORT, WHEEL_BL_PORT}, {-WHEEL_FR_PORT, -WHEEL_BR_PORT})                              \
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})                           \
            .withGains({0.00135, 0, 0.00001}, {0.00135, 0, 0.00001}, {0.001, 0, 0.0001})                                     \
            .withOdometry()                                                                                            \
            .buildOdometry();
