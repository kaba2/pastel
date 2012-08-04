#ifndef PASTEL_AUTOMATON_FWD_H
#define PASTEL_AUTOMATON_FWD_H

#include "pastel/sys/hash.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/object_forwarding.h"

#include <unordered_map>
#include <unordered_set>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class Automaton_Fwd
	{
	public:
		//! The start/final-labeling of the state.
		class StateLabel;

		//! The symbol-labeling of the transition.
		class TransitionLabel;

		//! The underlying graph.
		typedef Incidence_Graph<GraphType::Directed, 
			StateLabel, TransitionLabel> Graph;

		//! The states.
		/*!
		The states are the vertices of the graph.
		*/
		typedef typename Graph::Vertex_Iterator
			State_Iterator;
		typedef typename Graph::Vertex_ConstIterator
			State_ConstIterator;

		//! The user-data for the states.
		typedef typename AsClass<StateData>::type
			StateData_Class;

		//! The transitions.
		/*!
		The transitions are the edges of the graph, 
		labeled with symbols.
		*/
		typedef typename Graph::Edge_Iterator
			Transition_Iterator;
		typedef typename Graph::Edge_ConstIterator
			Transition_ConstIterator;

		//! The user-data for the transitions.
		typedef typename AsClass<TransitionData>::type
			TransitionData_Class;

		//! The set of final states.
		typedef std::list<State_ConstIterator>
			FinalSet;
		typedef typename FinalSet::iterator
			Final_Iterator;
		typedef typename FinalSet::const_iterator
			Final_ConstIterator;

		//! The set of start states.
		typedef std::list<State_ConstIterator>
			StartSet;
		typedef typename StartSet::iterator
			Start_Iterator;
		typedef typename StartSet::const_iterator
			Start_ConstIterator;
	};

}

#endif
