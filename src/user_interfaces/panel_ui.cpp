#include "SUGIPROJ/user_interfaces/panel_ui.h"

using namespace sugi;

panel_ui::panel_ui(leda::GraphWin& gw) : m_graphwin{gw}, m_panel{"Walk through steps"} {
    configurePanel();
}

int panel_ui::nextStep() {
    return m_graphwin.open_panel(m_panel);
}

void panel_ui::configurePanel() {
    m_panel.button("Stop", 0);
    m_panel.button("Move Step Backward", 1);
	m_panel.button("Move Step Forward", 2);
}