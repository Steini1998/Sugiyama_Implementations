#pragma once

#include "SUGIPROJ/sugiyama.h"


namespace sugi {
    
    class cycle_breaking_berger : public step {
    private:

        void run() override;

        leda::list<leda::edge> findOutEdges(leda::node);
        leda::list<leda::edge> findInEdges(leda::node);

        void updateNewEdgeList(leda::list<leda::edge>);

        void removeAllEdgesFromNodeInOldEdgeList(leda::node);

        void reverseEdgesFromFeedbackSet();
        
        leda::list<leda::edge> m_original_edge_list;
        leda::list<leda::edge> m_updated_edge_list;
    };

}