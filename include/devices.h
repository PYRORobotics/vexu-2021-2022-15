//
// Created by charles on 3/3/22.
//

#ifndef VEXU_2021_2022_15_DEVICES_H
#define VEXU_2021_2022_15_DEVICES_H

#include "main.h"

inline okapi::Controller master(okapi::ControllerId::master);
inline pros::Controller prosMaster(pros::E_CONTROLLER_MASTER);

inline pyro::controllerLCD masterLCD(prosMaster);

inline okapi::ControllerButton main_lift_btn(okapi::ControllerDigital::L2);
inline okapi::ControllerButton main_jaw_btn(okapi::ControllerDigital::L1);

inline okapi::ControllerButton back_lift_btn(okapi::ControllerDigital::R2);
inline okapi::ControllerButton back_jaw_btn(okapi::ControllerDigital::R1);


inline okapi::Motor leftLift(-1);
inline okapi::Motor rightLift(10);
inline okapi::Motor back(-20);

inline pyro::multi_state_lift main_lift(
        {
                okapi::Motor(1, true, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees),
                okapi::Motor(10, false, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees)
        },
        (1.0 / 6.0),
        {0, 20, 75},
        pyro::multi_state_lift::CYCLE
);

inline pyro::chassis chassis(
        {-3, 4, -5},    // Left motors are 3 & 4 // implicit constructor calls
        {7, -8, 9},     // Right motors are 1 & 2 (reversed)
        0.6666
);

inline pros::Imu imu(14);

inline pros::ADIDigitalIn jaws1Trigger('H');
inline pyro::jaws jaws1(12, jaws1Trigger, 360);


inline pros::ADIDigitalIn jaws2Trigger('G');
inline pyro::jaws jaws2(19, jaws2Trigger, 370);

inline okapi::ADIEncoder encoderLeft{'A', 'B', true};
inline okapi::ADIEncoder encoderRight{'C', 'D', true};

#endif //VEXU_2021_2022_15_DEVICES_H
