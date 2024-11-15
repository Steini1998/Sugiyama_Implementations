#pragma once

#include "SUGIPROJ/sugiyama.h"

namespace sugi {
    
    class remake_graph : public step {
    private:

        void run() override;
		
		void removeDummys();
		
		void reverseEdges();
        
    };

}