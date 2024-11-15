#include <iostream>

#include "LEDA/graphics/graphwin.h"
#include "LEDA/geo/point.h"


#include "SUGIPROJ/steps/cycle_breaking_with_dfs.h"


using namespace leda;


class DFS {
private:
	GraphWin& m_graphwin;
	const graph& m_graph;
	
	list<edge> m_tree;
	list<edge> m_forward;
	list<edge> m_cross;
	list<edge> m_backward;
	
	/* dfs_numbers[v] = -1  <==>  node v has not been visited yet
	 * dfs_numbers[v] = i, i > -1  <==>  node v has been visited as (i+1)-th element */
	node_array<int> m_dfs_numbers;
	/* completion_numbers[v] = -1  <==>  node v has not been completed yet
	 * completion_numbers[v] = i, i > -1  <==>  node v has been completion as (i+1)-th element */
	node_array<int> m_completion_numbers;
	
	int m_dfs_counter;
	int m_completion_counter;
	
	
	bool has_node_already_been_visited_by_dfs(node v) {
		return (m_dfs_numbers[v] > -1);
	}
	
	
	bool has_node_already_been_completed_by_dfs(node v) {
		return (m_completion_numbers[v] > -1);
	}
	
	
	void mark_node_as_visited(node v) {
		m_dfs_numbers[v] = m_dfs_counter++;
	}
	
	
	void mark_node_as_completed(node v) {
		m_completion_numbers[v] = m_completion_counter++;
	}
	
	
	bool is_tree(edge e) {
		node w = m_graph.target(e);
		return !this->has_node_already_been_visited_by_dfs(w);
	}
	
	
	bool is_forward(edge e) {
		node v = m_graph.source(e);
		node w = m_graph.target(e);
		return this->has_node_already_been_visited_by_dfs(w) 
			&& (m_dfs_numbers[v] < m_dfs_numbers[w]);
	}
	
	
	bool is_cross(edge e) {
		node v = m_graph.source(e);
		node w = m_graph.target(e);
		return this->has_node_already_been_visited_by_dfs(w)
			&& this->has_node_already_been_completed_by_dfs(w)
			&& (m_dfs_numbers[v] > m_dfs_numbers[w]);
	}
	
	
	bool is_backward(edge e) {
		node v = m_graph.source(e);
		node w = m_graph.target(e);
		return this->has_node_already_been_visited_by_dfs(w)
			&& !this->has_node_already_been_completed_by_dfs(w)
			&& (m_dfs_numbers[v] > m_dfs_numbers[w]);
	}
	
	
	list<edge> find_one_cycle() {
		// No cycle, so return empty list
		if (this->is_graph_acyclic()) { return list<edge>{}; }
		/* Now: graph is acyclic, so it definitely contains 1 backward-edge (a, b)
		 * So find path from b to a, and add backwards-edge */
		edge back_edge = m_backward.head();
		node v = m_graph.target(back_edge);
		node w = m_graph.source(back_edge);
		list<edge> edge_cycle {};
		find_edge_path(v, w, edge_cycle);
		edge_cycle.push_back(back_edge);
		return edge_cycle;
	}
	
	
	bool find_edge_path(node v, node w, list<edge>& c_edges) {
		if (v == w) { return true; }
		edge e;
		forall_out_edges(e, v) {
			c_edges.push_back(e);
			if (find_edge_path(m_graph.target(e), w, c_edges)) { return true; }
		}
		c_edges.pop_back();
		return false;
	}
	
	
public:
	DFS(GraphWin& gw) : m_graphwin(gw), m_graph(gw.get_graph()), 
						m_tree {}, m_forward {}, m_cross {}, m_backward {}, 
						m_dfs_numbers(m_graph, -1), m_completion_numbers(m_graph, -1), 
						m_dfs_counter(0), m_completion_counter(0) 
					{}
	
	
	void complete_dfs() {
		node v;
		forall_nodes(v, m_graph) {
			if (!this->has_node_already_been_visited_by_dfs(v)) {
				this->dfs_from_node(v);
			}
		}
	}
	
	
	void dfs_from_node(node v) { 
		this->mark_node_as_visited(v);
		edge e;
		forall_out_edges(e, v) {
			if (this->is_tree(e)) {
				m_tree.append(e);
				node w = m_graph.target(e);
				this->dfs_from_node(w);
			} else if (this->is_forward(e)) {
				m_forward.append(e);
			} else if (this->is_cross(e)) {
				m_cross.append(e);
			} else if (this->is_backward(e)) {
				m_backward.append(e);
			}
		}
		this->mark_node_as_completed(v);
	}
	
	
	bool is_graph_acyclic() {
		return m_backward.empty();
	}
	
	
	void order_nodes_in_graphwindow() {
		int x_pos, y_pos;
		int node_counter = m_graph.number_of_nodes();
		node v;
		forall_nodes(v, m_graph) {
			x_pos = (node_counter - m_completion_numbers[v]) * 50;
			y_pos = (node_counter - m_completion_numbers[v]) * 20;
			m_graphwin.set_position(v, point(x_pos, y_pos));
		}
	}
	
	
	void colorize_one_cycle_in_graphwindow() {
		list<edge> cycle_edges = this->find_one_cycle();
		edge e;
		forall(e, cycle_edges) {
			m_graphwin.set_color(e, blue);
		}
	}
	
	list<edge> getBackwardsEdges() {
		return m_backward;
	}
	
};


void sugi::cycle_breaking::run() {
    std::cout << "Cycle-Breaking" << std::endl;

    graph& g = m_sugiyama->getGraph();
	graph_update_tracker& gut = m_sugiyama->getGraphUpdateTracker();
	
	DFS dfs {m_sugiyama->getGraphWin()};
	dfs.complete_dfs();
	
	edge backwards_edge;
	forall(backwards_edge, dfs.getBackwardsEdges()) {
		edge reversed_edge = g.rev_edge(backwards_edge);
		gut.addReversedEdge(reversed_edge);
	}
}
