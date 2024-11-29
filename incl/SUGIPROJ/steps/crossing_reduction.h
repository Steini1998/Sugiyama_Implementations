#pragma once

#include <SUGIPROJ/sugiyama.h>

#include <LEDA/geo/segment.h>


namespace sugi {

    class crossing_reduction : public step {
	private:
	
		void run() override;
		
		void cross_reduction(leda::graph&, leda::node_array<leda::point>&);
	
        bool segments_intersect_excluding_endpoints(leda::segment, leda::segment);

        int count_edge_crossings(const leda::graph&, const leda::node_array<leda::point>&);

        bool swap_and_check(leda::graph&, leda::node_array<leda::point>&, leda::node, leda::node);

        void greedy_crossing_reduction(leda::graph&, leda::node_array<leda::point>&);

        double calculate_barycenter(const leda::graph&, const leda::node_array<leda::point>&, leda::node);

        void barycenter_crossing_reduction(leda::graph&, leda::node_array<leda::point>&);

        double calculate_median_x(const leda::graph&, const leda::node_array<leda::point>&, leda::node);

        void median_crossing_reduction(leda::graph&, leda::node_array<leda::point>&);
		
    };
	
}

