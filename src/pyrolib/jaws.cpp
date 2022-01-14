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
        
    }

    bool jaws::calibrate() {
        jawsMotor.moveVelocity(10);
        pros::delay(50);
        while(jawsMotor.getCurrentDraw() > 100){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(750);
        pros::delay(50);
        while(jawsMotor.getActualVelocity() > 2){
            pros::delay(10);
        }
        jawsMotor.tarePosition();
        state = CALIBRATED;
        return true;
    }

    bool jaws::open() {
        if(state == CALIBRATED || state == CLOSED){
            jawsMotor.moveAbsolute(235 * ((int) jawsMotor.getPosition() % 360), 100);
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
        if(!triggered && isTriggered()){
            return true;
        }
        else {
            return false;
        }
    }


}
