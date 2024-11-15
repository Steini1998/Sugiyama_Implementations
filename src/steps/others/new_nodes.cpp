#include <iostream>

#include "LEDA/graphics/graphwin.h"
#include "LEDA/geo/point.h"

#include "SUGIPROJ/sugiyama.h"
#include "SUGIPROJ/steps/new_nodes.h"


using namespace leda;


void sugi::new_nodes::run() {
    std::cout << "New-Nodes" << std::endl;

    graph& g = m_sugiyama->getGraph();
    node_map<point>& positions = m_sugiyama->getPositions();
    GraphWin& gw = m_sugiyama->getGraphWin();

    sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();

    node n = g.new_node();
    positions[n] = point(45, 38);

    node n2 = g.new_node();
    positions[n2] = point(80, 80);

    tracker.addAddedNode(n);
    tracker.addAddedNode(n2);
	
	edge e = g.new_edge(n, n2);
	edge e_rev = g.rev_edge(e);
	g.rev_edge(e_rev);

}
