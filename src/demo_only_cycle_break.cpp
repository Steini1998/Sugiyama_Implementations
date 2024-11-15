#include <stdio.h>

#include "LEDA/graphics/graphwin.h"

#include "SUGIPROJ/steps/cycle_breaking_berger.h"

#include "SUGIPROJ/sugiyama.h"

#include "SUGIPROJ/step_viewer.h"
#include "SUGIPROJ/user_interfaces/cmd_ui.h"


using namespace sugi;


int main() {
    leda::GraphWin gw {"Sugiyama"};
	
	step_viewer sv {};
    // sv.setStepUserInterface(new panel_ui{gw});
    sv.setStepUserInterface(new sugi::cmd_ui{});
	
    gw.display();

	while (gw.edit()) {
		sugi::sugiyama sg {gw};
		sg.setStepViewer(sv);
		
        sg.add(new sugi::cycle_breaking_berger{});

        sg.executeAllSteps();
        sg.viewAllSteps();
	}
    
    return 0;
}