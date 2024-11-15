#pragma once

#include "LEDA/graph/graph.h"

#include "SUGIPROJ/sugiyama.h"

namespace sugi {
    
    class leveling : public step {
    private:

        void run() override;
		
		leda::node position_empty(double, double);
		
		void longest_path(leda::node, leda::node_map<int>&);
		
		void dummy_nodes(leda::node_map<int>&, leda::list<leda::node>&);
		
		void determine_positions(leda::node_map<int>&, leda::list<leda::node>&);
        
    };

}