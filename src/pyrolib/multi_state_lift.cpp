//
// Created by mayowa on 4/22/22.
//

#include "../../include/pyrolib/include/multi_state_lift.h"

#include <utility>
#include <cmath>

namespace pyro {
    multi_state_lift::multi_state_lift(
            std::initializer_list<okapi::Motor> motors,
            double gear_ratio,
            std::vector<double> states,
            TRANSITION_TYPE ttype,
            unsigned int initial_state) : gear_ratio(gear_ratio),
                                          states(std::move(states)),
                                          ttype(ttype) {
        lift_motors = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup(motors));
        state_idx = initial_state;
    }

    void multi_state_lift::transition(unsigned int new_state) {
        switch (ttype) {
            case CYCLE:
                state_idx = new_state % states.size();
                break;
            case SWEEP:
                if (state_dir == FORWARD && state_idx == states.size() - 1) {
                    state_dir = REVERSE;
                    state_idx = states.size() - 2;
                } else if ((state_dir == REVERSE) && state_idx == 0) {
                    state_dir = FORWARD;
                    state_idx = 1;
                } else {
                    state_idx = new_state;
                }
                break;
            case HOLD:
                state_idx = static_cast<int>(fmax(0, fmin(new_state, states.size() - 1)));
                break;
        }

        double new_pos = states[state_idx];
        double calculated_pos = (1 / gear_ratio) * new_pos;

        printf("new state index: %d\n", state_idx);
        printf("calculated pos: %lf\n", calculated_pos);

        lift_motors->moveAbsolute(calculated_pos, 200);
    }

    void multi_state_lift::next() {
        transition(state_idx + 1);
    }

    void multi_state_lift::prev() {
        transition(state_idx - 1);
    }

    void multi_state_lift::step() {
        if (ttype == SWEEP) {
            printf("state_dir: %d\n", state_dir);
            state_dir == FORWARD
            ? next()
            : prev();
            return;
        }
        next();
    }

    unsigned int multi_state_lift::get_state() {
        return state_idx;
    }
}