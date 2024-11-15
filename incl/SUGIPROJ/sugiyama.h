#pragma once

#include "LEDA/graphics/graphwin.h"
#include "LEDA/core/list.h"

#include "SUGIPROJ/step.h"

#include "SUGIPROJ/graph_update_tracker.h"

#include "SUGIPROJ/step_viewer.h"

#include "SUGIPROJ/step_user_interface.h"


namespace sugi {
	
	class sugiyama {
	public:
		sugiyama(leda::GraphWin&);

		void add(step*);
		void remove(step*);

		void executeAllSteps(); // run all steps sequential
		void viewAllSteps(); // iterate over intermediate solutions

		leda::GraphWin& getGraphWin();
		
		leda::graph& getGraph();
		leda::node_map<leda::point>& getPositions();

		graph_update_tracker& getGraphUpdateTracker();

		void setStepViewer(const step_viewer&);

	private:
		leda::GraphWin& m_graphwin;

		/* Same graph and positions will be processed by steps. */
		leda::graph& m_graph;
		leda::node_map<leda::point> m_positions;
		
		/* Lists, that keep track of the changed graph (e.g. edges and nodes) for all steps to access */
		graph_update_tracker m_graph_update_tracker;

		leda::list<step*> m_steps;
		step_viewer m_step_viewer;
	
	};
	
}