#pragma once

namespace sugi {

    class step_user_interface {
    public:
        /* Returns 0, if view should stop at current step.
         * Returns 1, if view should go 1 step backward.
         * Returns 2, if view should go 1 step forward. */
        virtual int nextStep() = 0;  
    };

}