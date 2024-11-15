#pragma once

#include <LEDA/core/list.h>

#include <LEDA/graph/graph.h>

namespace sugi {

	class graph_update_tracker {
	public:
		graph_update_tracker();
		
		leda::list<leda::node>& getAddedNodes();
		void addAddedNode(leda::node);
		void removeAddedNode(leda::node);
		
		leda::list<leda::node>& getRemovedNodes();
		void addRemovedNode(leda::node);
		void removeRemovedNode(leda::node);
		
		leda::list<leda::edge>& getAddedEdges();
		void addAddedEdge(leda::edge);
		void removeAddedEdge(leda::edge);
		
		leda::list<leda::edge>& getRemovedEdges();
		void addRemovedEdge(leda::edge);
		void removeRemovedEdge(leda::edge);
		
		leda::list<leda::edge>& getReversedEdges();
		void addReversedEdge(leda::edge);
		void removeReversedEdge(leda::edge);

	private:
		leda::list<leda::node> m_added_nodes;
		leda::list<leda::node> m_removed_nodes;
		
		leda::list<leda::edge> m_added_edges;
		leda::list<leda::edge> m_removed_edges;
		leda::list<leda::edge> m_reversed_edges;
		
	};

}