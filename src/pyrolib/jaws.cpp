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
        jawsMotor.moveVelocity(30);
        pros::delay(50);
        while(jawsMotor.getCurrentDraw() > 100){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(-2000);
        pros::delay(50);
        while(abs(jawsMotor.getActualVelocity()) > 2){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(0);
        int absolute_pos = jawsMotor.getPosition();
        int relative_pos = (int) absolute_pos % 360;
        printf("old_rel_pos: %d\n", relative_pos);
        jawsMotor.tarePosition();
        state = CALIBRATED;
        return true;
    }

    bool jaws::open() {
        if(state == CALIBRATED || state == CLOSED){
            int absolute_pos = jawsMotor.getPosition();
            int revs = (int) absolute_pos / 360;
            int relative_pos = (int) absolute_pos % 360;
            if(relative_pos > 50){
                revs++;
            }
            int target_pos = 380 + (revs * 360);
            printf("abs_pos: %d\n", absolute_pos);
            printf("revs: %d\n", revs);
            printf("relative_pos: %d\n", relative_pos);
            printf("target_pos: %d\n", target_pos);
            jawsMotor.moveAbsolute(target_pos, 100);
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

    double jaws::getTemperature() {
        return jawsMotor.getTemperature();
    }


}
