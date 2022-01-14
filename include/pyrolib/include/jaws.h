//
// Created by charles on 1/12/22.
//

#ifndef VEXU_2021_2022_TESTBED_JAWS_H
#define VEXU_2021_2022_TESTBED_JAWS_H
#include "../../main.h"

namespace pyro {
    class jaws {
    public:
        jaws(okapi::Motor jawsMotor, pros::ADIDigitalIn jawsTrigger);
        bool calibrate();
        bool close();
        bool open();
        bool isTriggered();
        bool getNewTrigger();

    private:
        okapi::Motor jawsMotor;
        pros::ADIDigitalIn jawsTrigger;
        enum {
            UNKNOWN,
            CALIBRATED,
            CLOSED,
            OPEN
        } typedef jawsState;
        jawsState state;
        bool triggered;
    };
}


#endif //VEXU_2021_2022_TESTBED_JAWS_H
