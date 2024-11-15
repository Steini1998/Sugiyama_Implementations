#pragma once

#include "LEDA/graph/graph.h"
#include "LEDA/geo/point.h"

#include "SUGIPROJ/positionable_graph.h"


namespace sugi {

	class sugiyama; // Forward declaration
	 
	class step {		
	public:

		void execute(); // Template method

		void showResult();

		void setSugiyama(sugiyama*);

	private:
		virtual void run() = 0;

		void saveResult();

	protected:
		sugiyama* m_sugiyama;

		positionable_graph m_positionable_graph;
		
	};
	
}