#pragma once

#include "SUGIPROJ/step.h"
#include "SUGIPROJ/step_user_interface.h"

namespace sugi {
    
    class step_viewer {
    public:
        step_viewer() = default;

        void view();

        void setSteps(const leda::list<step*>&);
        void setStepUserInterface(step_user_interface*);

    private:
		void showCurrentStepsResult();

		void moveStepBackward();
		void moveStepForward();

    private:
        leda::list<step*> m_steps;
        leda::list_item m_current_step_item;

        step_user_interface* m_step_user_interface;

    };

}