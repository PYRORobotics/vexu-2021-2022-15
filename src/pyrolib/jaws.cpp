//
// Created by charles on 1/12/22.
//

#include "../../include/pyrolib/include/jaws.h"

#include <utility>
namespace pyro {
    jaws::jaws(okapi::Motor jawsMotor, pros::ADIDigitalIn jawsTrigger) : jawsMotor(std::move(jawsMotor)),
                                                                               jawsTrigger(jawsTrigger),
                                                                               state(UNKNOWN),
                                                                               triggered(false){
        jawsMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
        jawsMotor.setGearing(okapi::AbstractMotor::gearset::red);
        jawsMotor.setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
    }

    bool jaws::calibrate() {
        jawsMotor.moveVelocity(10);
        pros::delay(50);
        while(jawsMotor.getCurrentDraw() > 100){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(500);
        pros::delay(50);
        while(jawsMotor.getActualVelocity() > 2){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(0);
        jawsMotor.tarePosition();
        state = CALIBRATED;
        return true;
    }

    bool jaws::open() {
        if(state == CALIBRATED || state == CLOSED){
            printf("number:%d\n", 235 - ((int) jawsMotor.getPosition() % 360));
            jawsMotor.moveRelative(235 - ((int) jawsMotor.getPosition() % 360), 100);
            pros::delay(40);
            while(abs(jawsMotor.getActualVelocity()) > 0){
                pros::delay(10);
            }
            state = OPEN;
            return true;
        }
        else{
            return false;
        }
    }

    bool jaws::close() {
        if(state == OPEN) {
            jawsMotor.moveRelative(40, 100);
            pros::delay(500);
            jawsMotor.moveVoltage(-2000);
            state = CLOSED;
            return true;
        }
        else{
            return false;
        }
    }

    bool jaws::isTriggered() {
        triggered = !jawsTrigger.get_value();
        return triggered;
    }

    bool jaws::getNewTrigger() {
        //printf("button: %d, triggered: %d\n", jawsTrigger.get_value(), triggered);
        return jawsTrigger.get_new_press();
        if(!triggered && isTriggered()){
            return true;
        }
        else {
            return false;
        }
    }



}
