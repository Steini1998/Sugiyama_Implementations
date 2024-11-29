#include <SUGIPROJ/steps/crossing_reduction.h>

#include <LEDA/graph/graph.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/geo/segment.h>
#include <LEDA/core/list.h>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace leda;



void sugi::crossing_reduction::run() {
	std::cout << "Crossing Reduction" << std::endl;
	graph& g = m_sugiyama->getGraph();
	node_array<point>& positions = m_sugiyama->getPositions();

	cross_reduction(g, positions);
}

void sugi::crossing_reduction::cross_reduction(graph& G, node_array<point>& node_pos) {
	node_array<point> node_pos_greedy(G);
	node_array<point> node_pos_barycenter(G);
	node_array<point> node_pos_median(G);

	node_pos_greedy = node_pos;
	greedy_crossing_reduction(G, node_pos_greedy);
	int crossings_greedy = count_edge_crossings(G, node_pos_greedy);

	node_pos_barycenter = node_pos;
	barycenter_crossing_reduction(G, node_pos_barycenter);
	int crossings_barycenter = count_edge_crossings(G, node_pos_barycenter);

	node_pos_median = node_pos;
	median_crossing_reduction(G, node_pos_median);
	int crossings_median = count_edge_crossings(G, node_pos_median);

	if (crossings_greedy <= crossings_barycenter && crossings_greedy <= crossings_median) {
		node_pos = node_pos_greedy;
	}
	else if (crossings_barycenter <= crossings_greedy && crossings_barycenter <= crossings_median) {
		node_pos = node_pos_barycenter;
	}
	else {
		node_pos = node_pos_median;
	}
}

bool sugi::crossing_reduction::segments_intersect_excluding_endpoints(segment s1, segment s2) {
	point p1 = s1.source();
	point q1 = s1.target();
	point p2 = s2.source();
	point q2 = s2.target();

	if (p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2) {
		return false;
	}

	return s1.intersection(s2);
}

int sugi::crossing_reduction::count_edge_crossings(const graph& G, const node_array<point>& node_pos) {
	int crossings = 0;
	edge e1, e2;
	forall_edges(e1, G) {
		segment s1(node_pos[source(e1)], node_pos[target(e1)]);
		forall_edges(e2, G) {
			if (e1 == e2) continue;
			segment s2(node_pos[source(e2)], node_pos[target(e2)]);
			if (segments_intersect_excluding_endpoints(s1, s2)) {
				crossings++;
			}
		}
	}
	return crossings / 2;
}

bool sugi::crossing_reduction::swap_and_check(graph& G, node_array<point>& node_pos, node n1, node n2) {
	int initial_crossings = count_edge_crossings(G, node_pos);

	point temp = node_pos[n1];
	node_pos[n1] = node_pos[n2];
	node_pos[n2] = temp;

	int new_crossings = count_edge_crossings(G, node_pos);

	if (new_crossings >= initial_crossings) {
		node_pos[n2] = node_pos[n1];
		node_pos[n1] = temp;
		return false;
	}

	return true;
}

void sugi::crossing_reduction::greedy_crossing_reduction(graph& G, node_array<point>& node_pos) {
	int yCoord = 50;

	while (true) {
		std::vector<node> nodes_in_layer;

		node v;
		forall_nodes(v, G) {
			if (node_pos[v].ycoord() == yCoord) {
				nodes_in_layer.push_back(v);
			}
		}

		if (nodes_in_layer.empty()) {
			break;
		}

		std::sort(nodes_in_layer.begin(), nodes_in_layer.end(), [&node_pos](const node& a, const node& b) {
			return node_pos[a].xcoord() < node_pos[b].xcoord();
			});

		for (size_t i = 0; i < nodes_in_layer.size() - 1; ++i) {
			node u = nodes_in_layer[i];
			node w = nodes_in_layer[i + 1];

			if (node_pos[w].xcoord() == node_pos[u].xcoord() + 50) {
				swap_and_check(G, node_pos, u, w);
			}
		}
		yCoord += 50;
	}
}

double sugi::crossing_reduction::calculate_barycenter(const graph& G, const node_array<point>& node_pos, node v) {
	double barycenter_x = 0;
	int degree = 0;

	edge e;
	forall_out_edges(e, v) {
		node u = target(e);
		barycenter_x += node_pos[u].xcoord();
		degree++;
	}

	if (degree > 0) {
		barycenter_x /= degree;
	}

	return barycenter_x;
}

void sugi::crossing_reduction::barycenter_crossing_reduction(graph& G, node_array<point>& node_pos) {
	node_array<double> y_coords(G);

	node v;
	forall_nodes(v, G) {
		y_coords[v] = node_pos[v].ycoord();
	}

	double max_y = -1;
	forall_nodes(v, G) {
		if (y_coords[v] > max_y) {
			max_y = y_coords[v];
		}
	}

	for (double y = max_y; y >= 0; y -= 50) {
		list<node> nodes_at_y;
		forall_nodes(v, G) {
			if (y_coords[v] == y) {
				nodes_at_y.append(v);
			}
		}

		nodes_at_y.sort([&](node n1, node n2) {
			return calculate_barycenter(G, node_pos, n1) < calculate_barycenter(G, node_pos, n2);
			});

		double last_x = -std::numeric_limits<double>::infinity();
		node u;
		forall(u, nodes_at_y) {
			double new_x = calculate_barycenter(G, node_pos, u);
			if (new_x <= last_x) {
				new_x = last_x + 50;
			}
			node_pos[u] = point(new_x, y);
			last_x = new_x;
		}
	}
}

double sugi::crossing_reduction::calculate_median_x(const graph& G, const node_array<point>& node_pos, node v) {
	list<double> x_coords;

	edge e;
	forall_out_edges(e, v) {
		node u = target(e);
		x_coords.append(node_pos[u].xcoord());
	}

	x_coords.sort();

	int size = x_coords.size();
	if (size == 0) {
		return 0;
	}

	int median_index = size / 2;
	list_item it = x_coords.first();
	for (int i = 0; i < median_index; ++i) {
		it = x_coords.succ(it);
	}

	if (size % 2 == 1) {
		return x_coords[it];
	}
	else {
		double first_median = x_coords[it];
		double second_median = x_coords[x_coords.pred(it)];
		return (first_median + second_median) / 2.0;
	}
}

void sugi::crossing_reduction::median_crossing_reduction(graph& G, node_array<point>& node_pos) {
	node_array<double> y_coords(G);

	node v;
	forall_nodes(v, G) {
		y_coords[v] = node_pos[v].ycoord();
	}

	double max_y = -1;
	forall_nodes(v, G) {
		if (y_coords[v] > max_y) {
			max_y = y_coords[v];
		}
	}

	for (double y = max_y; y >= 0; y -= 50) {
		list<node> nodes_at_y;
		forall_nodes(v, G) {
			if (y_coords[v] == y) {
				nodes_at_y.append(v);
			}
		}

		nodes_at_y.sort([&](node n1, node n2) {
			return calculate_median_x(G, node_pos, n1) < calculate_median_x(G, node_pos, n2);
			});

		double last_x = -std::numeric_limits<double>::infinity();
		node u;
		forall(u, nodes_at_y) {
			double new_x = calculate_median_x(G, node_pos, u);
			if (new_x <= last_x) {
				new_x = last_x + 50;
			}
			node_pos[u] = point(new_x, y);
			last_x = new_x;
		}
	}
}

