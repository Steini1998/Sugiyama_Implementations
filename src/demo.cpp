#include <stdio.h>

#include "LEDA/graphics/graphwin.h"

#include "SUGIPROJ/steps/input.h"
#include "SUGIPROJ/steps/cycle_breaking_berger.h"
#include "SUGIPROJ/steps/leveling.h"
#include "SUGIPROJ/steps/remake_graph.h"

#include "SUGIPROJ/sugiyama.h"

#include "SUGIPROJ/step_viewer.h"
#include "SUGIPROJ/user_interfaces/panel_ui.h"


using namespace sugi;


int main() {
    leda::GraphWin gw {"Sugiyama"};
	
	step_viewer sv {};
    sv.setStepUserInterface(new panel_ui{gw});
	
    gw.display();

	while (gw.edit()) {
		sugi::sugiyama sg {gw};
		sg.setStepViewer(sv);
		
		sg.add(new sugi::input{});
        sg.add(new sugi::cycle_breaking_berger{});
        sg.add(new sugi::leveling{});
		sg.add(new sugi::remake_graph{});

        sg.executeAllSteps();
        sg.viewAllSteps();
	}
    
    return 0;
}