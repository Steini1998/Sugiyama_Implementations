#pragma once

#include <SUGIPROJ/sugiyama.h>

namespace sugi {

    class vertex_positioning : public step {
	public:
		vertex_positioning();
	
	private:
		void run() override;
	
        void straighten_edges();
		
		void halve_x_coordinates();

	private:
		leda::graph& G;
		leda::node_array<leda::point>& node_pos;

    };
	
}






