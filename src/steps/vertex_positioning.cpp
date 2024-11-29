#include <SUGIPROJ/steps/vertex_positioning.h>

#include <LEDA/graph/graph.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/geo/segment.h>
#include <LEDA/core/list.h>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace leda;


sugi::vertex_positioning::vertex_positioning() : G {m_sugiyama->getGraph()}, node_pos {m_sugiyama->getPositions()} {}


void sugi::vertex_positioning::run() {
	straighten_edges();
	//halve_x_coordinates();
}


void sugi::vertex_positioning::straighten_edges() {
	int yCoord = 50;

	while (true) {
		list<node> nodes_in_layer;
		node v;

		forall_nodes(v, G) {
			if (node_pos[v].ycoord() == yCoord) {
				nodes_in_layer.append(v);
			}
		}

		if (nodes_in_layer.empty()) {
			break;
		}

		nodes_in_layer.sort([&](const node& a, const node& b) {
			return node_pos[a].xcoord() < node_pos[b].xcoord();
			});

		int xCoord = 50;
		node n;
		forall(n, nodes_in_layer) {
			node_pos[n] = point(xCoord, yCoord);
			xCoord += 50;
		}

		yCoord += 50;
	}
}


void sugi::vertex_positioning::halve_x_coordinates() {
	node v;
	forall_nodes(v, G) {
		node_pos[v] = point(node_pos[v].xcoord() / 2, node_pos[v].ycoord());
	}
}






