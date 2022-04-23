//
// Created by mayowa on 4/22/22.
//

#ifndef VEXU_2021_2022_24_MULTI_STATE_LIFT_H
#define VEXU_2021_2022_24_MULTI_STATE_LIFT_H

#include "../../main.h"

namespace pyro {
    class multi_state_lift {
    public:

        /*
         * Marks how the lift should transition when it reaches the final state.
         * CYCLE will move to the first state (0, 1, 2, 0, 1, ...)
         * SWEEP will move to the previous state (0, 1, 2, 1, 0, ...)
         * HOLD will stop at the final state (0, 1, 2). Expected that you handle transitions in both directions.
         */
        enum TRANSITION_TYPE {
            CYCLE,
            SWEEP,
            HOLD
        };

        /*
         * The direction the lift will travel in. Relevant only for SWEEP transition type.
         */
        enum STATE_DIRECTION {
            FORWARD,
            REVERSE
        };

        multi_state_lift(
                std::initializer_list<okapi::Motor> motors,
                double gear_ratio,
                std::vector<double> states,
                TRANSITION_TYPE ttype = CYCLE,
                unsigned int initial_state = 0
        );

        void transition(unsigned int new_state);

        void next();

        void prev();

        void step();

    private:
        std::shared_ptr<okapi::MotorGroup> lift_motors;
        double gear_ratio;
        std::vector<double> states;
        unsigned int state_idx;
        TRANSITION_TYPE ttype;
        STATE_DIRECTION state_dir = FORWARD;
    };
}
#endif //VEXU_2021_2022_24_MULTI_STATE_LIFT_H
