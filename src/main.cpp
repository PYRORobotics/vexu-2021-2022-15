#include "main.h"

using namespace okapi;
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

    pros::delay(50);
    while(imu.is_calibrating()){
        pros::delay(20);
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
[[noreturn]] void opcontrol() {
    okapi::Controller master(okapi::ControllerId::master);
    pros::Controller prosMaster(pros::E_CONTROLLER_MASTER);

    pyro::controllerLCD masterLCD(prosMaster);

	okapi::Motor leftLift(-1);
    leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    okapi::Motor rightLift(10);
    rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    okapi::Motor arm(6);
    arm.setEncoderUnits(AbstractMotor::encoderUnits::degrees);

    pyro::chassis chassis(
                    {-3, 4, -5},    // Left motors are 3 & 4
                    {7, -8, 9}     // Right motors are 1 & 2 (reversed)
    );
    chassis.chassisController->getModel()->setBrakeMode(AbstractMotor::brakeMode::hold);


    pros::ADIDigitalIn jaws1Trigger('H');
    pyro::jaws jaws1(11, jaws1Trigger);

    pros::ADIDigitalIn jaws2Trigger('A');
    pyro::jaws jaws2(6, jaws2Trigger);


    imu.tare();

    bool armOut = false;
    bool frontLiftUp = true;
    bool backLiftUp = true;
	while (true) {

        //masterLCD.setControllerLCD(0, "ctemp: " + std::to_string(jaws1.getTemperature()));

        masterLCD.setControllerLCD(0, "degR: " + std::to_string(imu.get_roll()));
        masterLCD.setControllerLCD(1, "degY: " + std::to_string(imu.get_yaw()));
        masterLCD.setControllerLCD(2, "pos: " + std::to_string(leftLift.getPosition()));

        if(fabs(imu.get_roll()) > 33){
        //if(false){
            chassis.arcade((imu.get_roll() > 0) ? 1 : -1, 0, 0);
        }
        else {
            if (fabs(master.getAnalog(ControllerAnalog::leftY)) > 0.05 ||
                fabs(master.getAnalog(ControllerAnalog::rightY)) > 0.05) {
                //printf("got here\n");
                chassis.setCurrentLimit(2500);
                chassis.tank(master.getAnalog(ControllerAnalog::leftY),
                             master.getAnalog(ControllerAnalog::rightY), 0.05);
            } else {
                //chassis.tank(0, 0, 0.05);
                //chassis.setCurrentLimit(0);
                chassis.chassisController->getModel()->stop();
            }

        }

        /*
        if(master.getDigital(ControllerDigital::L1)){
            backLift.setCurrentLimit(2500);
            frontLift.setCurrentLimit(2500);
            backLift.moveVelocity(200);
            frontLift.moveVelocity(200);
        }
        else if(master.getDigital(ControllerDigital::L2)){
            backLift.setCurrentLimit(2500);
            frontLift.setCurrentLimit(2500);
            backLift.moveVelocity(-200);
            frontLift.moveVelocity(-200);
        }
        else{
            backLift.setCurrentLimit(0);
            frontLift.setCurrentLimit(0);
            backLift.moveVelocity(0);
            frontLift.moveVelocity(0);
        }*/

        /*
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
            frontLiftUp = !frontLiftUp;
            if(frontLiftUp){
                frontLift.moveAbsolute(-50, 200);
            }
            else{
                frontLift.moveAbsolute(-4600, 200);
            }
        }
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
            backLiftUp = !backLiftUp;
            if(backLiftUp){
                backLift.moveAbsolute(-50, 200);
            }
            else{
                backLift.moveAbsolute(-4600, 200);
            }
        }*/


        if(master.getDigital(ControllerDigital::L1)){
            if(leftLift.getPosition() < 3100){
                leftLift.setCurrentLimit(2500);
                leftLift.moveVelocity(200);
            }
            else{
                leftLift.moveVelocity(0);
            }

            if(rightLift.getPosition() < 3100){
                rightLift.setCurrentLimit(2500);
                rightLift.moveVelocity(200);
            }
            else{
                rightLift.moveVelocity(0);
            }
        }
        else if(master.getDigital(ControllerDigital::L2)){
            if(leftLift.getPosition() > 0){
                leftLift.setCurrentLimit(2500);
                leftLift.moveVelocity(-200);
            }
            else{
                leftLift.setCurrentLimit(0);
                leftLift.moveVelocity(0);
            }

            if(rightLift.getPosition() > 0){
                rightLift.setCurrentLimit(2500);
                rightLift.moveVelocity(-200);
            }
            else{
                rightLift.setCurrentLimit(0);
                rightLift.moveVelocity(0);
            }
        }
        else{
            //rightLift.setCurrentLimit(0);
            //leftLift.setCurrentLimit(0);
            rightLift.moveVelocity(0);
            leftLift.moveVelocity(0);
        }

        if(master.getDigital(okapi::ControllerDigital::Y)){
            jaws1.calibrate();
        }
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
            jaws1.open();
        }
        if(jaws1.getNewTrigger() || master.getDigital(okapi::ControllerDigital::R2)){
            jaws1.close();
        }

        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
            if(armOut){
                arm.moveRelative(-180,100);
            }
            else{
                arm.moveRelative(180, 100);
            }
            armOut = !armOut;
        }

        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
            printf("arm_pos: %d\n", (int)rightLift.getPosition());
        }
		pros::delay(10);
	}
}
