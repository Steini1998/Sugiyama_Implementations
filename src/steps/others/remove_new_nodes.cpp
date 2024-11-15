#include <iostream>

#include "LEDA/graphics/graphwin.h"
#include "LEDA/geo/point.h"

#include "SUGIPROJ/sugiyama.h"
#include "SUGIPROJ/steps/remove_new_nodes.h"


using namespace leda;


void sugi::remove_new_nodes::run() {
    std::cout << "Remove-New-Nodes" << std::endl;

    graph& g = m_sugiyama->getGraph();
    sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();

    node n;
    forall (n, tracker.getAddedNodes()) {
        g.del_node(n);
    }
}
