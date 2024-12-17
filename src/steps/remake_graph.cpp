#include <iostream>

#include <LEDA/graphics/graphwin.h>
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>

#include <SUGIPROJ/steps/remake_graph.h>

using namespace leda;


void sugi::remake_graph::run(){
	std::cout << "Remake Graph" << std::endl;
	removeDummys();
	addRemovedEdges();
	reverseEdges();
}
   
   
void sugi::remake_graph::removeDummys(){
	graph& G = m_sugiyama->getGraph();
	sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();
	node dummy_node;
	forall(dummy_node, tracker.getAddedNodes()){
		G.del_node(dummy_node);
	}
}


void sugi::remake_graph::addRemovedEdges(){
	graph& G = m_sugiyama->getGraph();
	sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();
	edge e;
	forall(e, tracker.getRemovedEdges()){
		G.new_edge(G.source(e), G.target(e));
	}
}


void sugi::remake_graph::reverseEdges(){
	graph& G = m_sugiyama->getGraph();
	sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();
	edge e;
	forall(e, tracker.getReversedEdges()){
		node source = G.source(e);
		node target = G.target(e);
		// suche nach entsprechender Kante
		edge e1;
		forall_adj_edges(e1, target){
			if (G.target(e1) == source){
				G.del_edge(e1);
				break;
			}
		}
		G.new_edge(target, source);
	}
}