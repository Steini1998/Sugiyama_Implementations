#pragma once

#include "LEDA/graphics/graphwin.h"
#include "LEDA/graphics/panel.h"

#include "SUGIPROJ/step_user_interface.h"

namespace sugi {
    
    class panel_ui : public step_user_interface {
    public:
        panel_ui(leda::GraphWin&);

        int nextStep() override;

    private:
        void configurePanel();

    private:
        leda::GraphWin& m_graphwin;
        leda::panel m_panel;
        
    };

}