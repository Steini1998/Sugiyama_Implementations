#include <iostream>

#include "SUGIPROJ/steps/cycle_breaking_berger.h"

using namespace sugi;


/* Implemented GreedyMakeAcyclic-Algorithm */
void cycle_breaking_berger::run() {
    leda::graph& G = m_sugiyama->getGraph();
    m_original_edge_list = G.all_edges();
    leda::node v;
    forall_nodes(v, G) {
        leda::list<leda::edge> outEdges = findOutEdges(v);
        leda::list<leda::edge> inEdges = findInEdges(v);

        if (outEdges.length() >= inEdges.length()) {
            updateNewEdgeList(outEdges);
        } else {
            updateNewEdgeList(inEdges);
        }
        removeAllEdgesFromNodeInOldEdgeList(v);
    }
    reverseEdgesFromFeedbackSet();
}

leda::list<leda::edge> cycle_breaking_berger::findOutEdges(leda::node n) {
    leda::list<leda::edge> outEdges {};
    leda::edge e;
    forall(e, m_original_edge_list) {
        if (m_sugiyama->getGraph().source(e) == n) {
            outEdges.push_back(e);
        }
    }
    return outEdges;
}

leda::list<leda::edge> cycle_breaking_berger::findInEdges(leda::node n) {
    leda::list<leda::edge> inEdges {};
    leda::edge e;
    forall(e, m_original_edge_list) {
        if (m_sugiyama->getGraph().target(e) == n) {
            inEdges.push_back(e);
        }
    }
    return inEdges;
}

void cycle_breaking_berger::updateNewEdgeList(leda::list<leda::edge> new_edges) {
    leda::edge e;
    forall (e, new_edges) {
        m_updated_edge_list.push_back(e);
    }
}

void cycle_breaking_berger::removeAllEdgesFromNodeInOldEdgeList(leda::node n) {
    leda::edge e;
    forall_inout_edges (e, n) {
        m_original_edge_list.remove(e);
    }
}

void cycle_breaking_berger::reverseEdgesFromFeedbackSet() {
    leda::graph& G = m_sugiyama->getGraph();
    sugi::graph_update_tracker& gut = m_sugiyama->getGraphUpdateTracker();
    leda::edge e;
    forall (e, G.all_edges()) {
        if (m_updated_edge_list.search(e) == nil) {
            leda::edge reversed_edge = G.rev_edge(e);
            gut.addReversedEdge(reversed_edge);
        }
    }
}