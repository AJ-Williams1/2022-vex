#include "main.h"
#include "ports.h"

using namespace okapi;

#define setup_flywheel                                                                                                 \
    std::shared_ptr<AsyncVelocityController<double, double>> flyCtrl =                                                 \
        AsyncVelControllerBuilder()                                                                                    \
            .withMotor({-FLY1_PORT, FLY2_PORT})                                                                        \
            .withGains({0.001, 0.0, 0.001, 0.0})                                                                       \
            .withVelMath(VelMathFactory::createPtr(gearsetToTPR(AbstractMotor::gearset::green)))                       \
            .build();

#define setup_chassis                                                                                                  \
    std::shared_ptr<ChassisController> drive =                                                                         \
        ChassisControllerBuilder()                                                                                     \
            .withMotors({WHEEL_FL_PORT, WHEEL_BL_PORT}, {-WHEEL_FR_PORT, -WHEEL_BR_PORT})                              \
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})                           \
            .withGains({0.001, 0, 0.0001}, {0.001, 0, 0.0001}, {0.001, 0, 0.0001})                                     \
            .withOdometry()                                                                                            \
            .buildOdometry();
