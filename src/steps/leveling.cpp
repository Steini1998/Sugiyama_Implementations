//Schichtzuweisung

#include <LEDA/graphics/graphwin.h>
#include <LEDA/graph/graph.h>
#include <LEDA/core/queue.h>

#include <SUGIPROJ/steps/leveling.h>


using namespace leda;


void sugi::leveling::run() {
	std::cout << "Leveling" << std::endl;

	graph& g = m_sugiyama->getGraph();

	list<node> dummy_list;

	node_map<int> level(g,0);
	node v;
	forall_nodes(v,g){
		if (g.indeg(v) == 0){
			longest_path(v, level);
		}
	}
	dummy_nodes(level,dummy_list);
	determine_positions(level, dummy_list);

	node n;
	forall(n, m_sugiyama->getGraphUpdateTracker().getAddedNodes()) {
		std::cout << m_sugiyama->getGraph().index(n) << std::endl;
	}
	edge e;
	forall(e, m_sugiyama->getGraphUpdateTracker().getRemovedEdges()) {
		node s = m_sugiyama->getGraph().source(e);
		node t = m_sugiyama->getGraph().target(e);
		std::cout << m_sugiyama->getGraph().index(e) << " : "<< m_sugiyama->getGraph().index(s) << " " << m_sugiyama->getGraph().index(t) << std::endl;
	}
	
}

    
// Teste, ob an einer Koordinate schon ein Knoten gesetzt wurde
node sugi::leveling::position_empty(double x, double y){
	node n;
	forall_nodes(n, m_sugiyama->getGraph()){
		point pos = m_sugiyama->getPositions()[n];
		if (pos.xcoord() == x && pos.ycoord() == y){
			return n;
		}
	}
	return nil;
}


//Level Berechnung
void sugi::leveling::longest_path(node s, node_map<int>& level){
	queue<node> Q;
	Q.append(s); 
	level[s] = 0;

	while (!Q.empty()){
		node v = Q.pop();
		node u;
		forall_adj_nodes(u,v){
			Q.append(u);
			if (level[u] < (level[v]+ 1)){
				level[u] = level[v] + 1;
			}
		}
	}
}


//Dummy nodes für Kanten, die über mehrere Levels gehen
void sugi::leveling::dummy_nodes(node_map<int>& level, list<node>& dummy_list){
	graph& G = m_sugiyama->getGraph();
	sugi::graph_update_tracker& tracker = m_sugiyama->getGraphUpdateTracker();
	edge e;
	list<edge> to_remove;
	forall_edges(e,G){
		node source = G.source(e);
		node target = G.target(e);
		int lvlDiff = level[target] - level[source];
		
		if (lvlDiff > 1){
			tracker.addRemovedEdge(e);
			node prev = source;
			//Einfügen der Dummy Nodes
			for (int i = 1; i < lvlDiff; i++){
				node dummy = G.new_node();
				tracker.addAddedNode(dummy);
				GraphWin& gw = m_sugiyama->getGraphWin();
				// gw.set_node_shape(rectangle_node, dummy);
				//Neue Level zuweisen
				level[dummy] = level[source] + i;
				// Kante von vorherigem Knoten zu dummy Knoten
				G.new_edge(prev,dummy);
				prev = dummy;
			}
			// Kante vom letzen dummy Knoten zu Zielknoten
			G.new_edge(prev,target);
		}
	}
	//Remove original edges
	edge e_remove;
	forall(e_remove, tracker.getRemovedEdges()){
		G.del_edge(e_remove);
	}
}


//Levelzeichnung
void sugi::leveling::determine_positions(node_map<int>& level, list<node>& dummy_list){
	node_map<point>& positions = m_sugiyama->getPositions();
	const graph& G = m_sugiyama->getGraph();
	node v;
	double x = 50;
	double y = 50;
	forall_nodes(v, G){
		double z = y + level[v]* 50;
		while (position_empty(x, z) != nil){// Knoten an leere Position setzen
			x += 50.0;
		}
		std::cout << x << " : " << z << std::endl;
		positions[v] = point(x,z);
		//Anpassung Knotenform Dummy Knoten
		
		x = 50;
	}
}


