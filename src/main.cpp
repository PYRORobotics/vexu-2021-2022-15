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
void autonomous() {}

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
	okapi::Motor rightLift(-5);
    rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    okapi::Motor leftLift(15);
    leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    okapi::Motor arm(6);
    arm.setEncoderUnits(AbstractMotor::encoderUnits::degrees);

    std::shared_ptr<okapi::ChassisController> chassis = okapi::ChassisControllerBuilder()
            .withMotors(

                    {-11, 12, -13},    // Left motors are 3 & 4
                    {1, -2, 3}     // Right motors are 1 & 2 (reversed)
            )
            .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 11.5_in}, okapi::imev5GreenTPR})
            .build();

    pros::ADIDigitalIn jawsTrigger('B');
    pyro::jaws jaws1(18, jawsTrigger);


    bool armOut = false;
	while (true) {
        chassis->getModel()->tank(master.getAnalog(ControllerAnalog::leftY), master.getAnalog(ControllerAnalog::rightY), 0.05);

        if(master.getDigital(ControllerDigital::L1)){
            leftLift.moveVelocity(200);
            rightLift.moveVelocity(200);
        }
        else if(master.getDigital(ControllerDigital::L2)){
            leftLift.moveVelocity(-200);
            rightLift.moveVelocity(-200);
        }
        else{
            leftLift.moveVelocity(0);
            rightLift.moveVelocity(0);
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
		pros::delay(10);
	}
}
