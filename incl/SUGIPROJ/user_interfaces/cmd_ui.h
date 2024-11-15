#pragma once

#include "SUGIPROJ/step_user_interface.h"

namespace sugi {
    
    class cmd_ui : public step_user_interface {
    public:
        int nextStep() override;
    };

}
