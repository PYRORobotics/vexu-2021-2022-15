//
// Created by charles on 9/30/21.
//

#include "pyrolib/include/chassis.h"

#include <memory>
namespace pyro {
    void chassis::arcade(double forward, double turn, double threshold) {
        chassisController->getModel()->arcade(forward, turn, threshold);
    }

    void chassis::tank(double left, double right, double threshold) {
        chassisController->getModel()->tank(left, right, threshold);
    }

    void chassis::setCurrentLimit(int milliamps){
        for(okapi::Motor motor : leftMotorsVector){
            motor.setCurrentLimit(milliamps);
        }
        for(okapi::Motor motor : rightMotorsVector){
            motor.setCurrentLimit(milliamps);
        }
    }

    chassis::chassis(std::initializer_list<okapi::Motor> leftMotorsList,
                           std::initializer_list<okapi::Motor> rightMotorsList,
                           double gearRatio,
                           okapi::QLength wheelDiameter,
                           okapi::QLength wheelWidth,
                           const std::int32_t TPR) {
        leftMotorsVector = leftMotorsList;
        rightMotorsVector = rightMotorsList;

        leftMotors = std::make_shared<okapi::MotorGroup>(leftMotorsList);
        rightMotors = std::make_shared<okapi::MotorGroup>(rightMotorsList);

        chassisController = okapi::ChassisControllerBuilder()
                .withMotors(
                        leftMotorsList, // Left motors are 1 & 2 (reversed)
                        rightMotorsList    // Right motors are 3 & 4
                )
                .withDimensions({okapi::AbstractMotor::gearset::green, gearRatio},
                                {{wheelDiameter, wheelWidth}, static_cast<double>(TPR)})
                .build();
    }

}
