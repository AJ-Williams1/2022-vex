#include "autoSelect/selection.h"
#include "main.h"
#include "ports.h"
#include "setup_macros.h"

using namespace okapi;

// Constants
#define WHEEL_DIAMETER 4.0
#define WHEELBASE_WIDTH 11.5
#define WHEEL_TO_EDGE 2

// Function declarations
void color_quick_spin();
void quick_shooter();

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


void tripleAuto(){       // shoot 3 in auton
  for (int i = 0; i < 3; i++){
    while(!canFire()){
    } 
    if(canFire()){
      index_disc();
    }

  }
}

void doubleAuto(){       // shoot 2 in auton
  for (int i = 0; i < 2; i++){
    while(!canFire()){
     
    } 
    if(canFire()) index_disc();
    pros::delay(100);
  }
}
void singleAuto(){       // shoot 1 in auton
  for (int i = 0; i < 1; i++){
    while(!canFire()){
    } 
    if(canFire()) index_disc();
  }
}


void autonomous()
{
    // pros::Task flywheel_on(flywheel_on_fn);
    // flywheel_speed = 102;
    // pros::delay(2000);
    color_quick_spin();
    // quick_shooter();

    pros::Task flywheel_on(flyCalc, nullptr);
    setFlyAuto(58);
    pros::delay(100);
    color_quick_spin();
    doubleAuto();

    
}

// Helper functions

void color_quick_spin()
{
    setup_chassis;
    Motor colorwheel(COLORWHEEL_PORT);

    drive->getModel()->forward(50);
    pros::delay(200);
    drive->getModel()->stop();

    colorwheel.moveVoltage(6000);
    pros::delay(250);
    colorwheel.moveVoltage(0);
}

void quick_shooter()
{
    
    setup_chassis;
    Motor intake(-INTAKE_PORT);
    Motor fly1(FLY1_PORT);

    // pros::Task flywheel_on(flywheel_on_fn);
    // flywheel_speed = 100;

    pros::delay(5000);
    while ((((int)fly1.getActualVelocity()-flywheel_speed )> 5) or ((flywheel_speed- (int)fly1.getActualVelocity())> 5))  
    {

    }
    index_disc();
    intake.moveVoltage(1200);
    pros::delay(500);
    //flywheel_speed = 95;
    while ((((int)fly1.getActualVelocity()-flywheel_speed )> 5) or ((flywheel_speed- (int)fly1.getActualVelocity())> 5))  
    {
        
    }
    index_disc();
    intake.moveVoltage(0);
        


}
