#pragma once

#include <LEDA/graphics/graphwin.h>

#include <LEDA/graph/graph.h>
#include <LEDA/geo/point.h>


namespace sugi {

	class positionable_graph {
	public:
		positionable_graph() = default;

		void takeSnapshot(const leda::graph&, const leda::node_map<leda::point>&);

		void show(leda::GraphWin&);

	private:
		void mapPositions(const leda::graph&, const leda::node_map<leda::point>&);
		
	private:
		leda::graph m_graph; // independent copy from graph passed to 'takeSnapshot'
		leda::node_array<leda::point> m_positions; // independent static copy from nodes passed to 'takeSnapshot'
		
	};

}