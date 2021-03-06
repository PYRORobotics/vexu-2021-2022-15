//
// Created by charles on 3/3/22.
//

#ifndef VEXU_2021_2022_15_DEVICES_H
#define VEXU_2021_2022_15_DEVICES_H
#include "main.h"

inline okapi::Controller master(okapi::ControllerId::master);
inline pros::Controller prosMaster(pros::E_CONTROLLER_MASTER);

inline pyro::controllerLCD masterLCD(prosMaster);

inline okapi::Motor leftLift(-1);
inline okapi::Motor rightLift(10);
inline okapi::Motor back(-20);

inline pyro::chassis chassis(
        {-3, 4, -5},    // Left motors are 3 & 4
        {7, -8, 9},     // Right motors are 1 & 2 (reversed)
        0.6666
);

inline pros::Imu imu(14);

inline pros::ADIDigitalIn jaws1Trigger('H');
inline pyro::jaws jaws1(11, jaws1Trigger, 360);


inline pros::ADIDigitalIn jaws2Trigger('G');
inline pyro::jaws jaws2(19, jaws2Trigger, 370);

inline okapi::ADIEncoder encoderLeft{'A', 'B', true};
inline okapi::ADIEncoder encoderRight{'C', 'D', true};

#endif //VEXU_2021_2022_15_DEVICES_H
