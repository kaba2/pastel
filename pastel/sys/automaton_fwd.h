#ifndef PASTEL_AUTOMATON_FWD_H
#define PASTEL_AUTOMATON_FWD_H

#include "pastel/sys/mytypes.h"

#include <pastel/sys/hash.h>
#include <pastel/sys/incidence_graph.h>

#include <list>
#include <unordered_map>

namespace Pastel
{

	template <typename State, typename Symbol>
	class Automaton_Fwd
	{
	public:
		typedef Incidence_Graph<GraphType::Directed, State, Symbol> Graph;

		typedef typename Graph::Vertex_Iterator
			State_Iterator;
		typedef typename Graph::Vertex_ConstIterator
			State_ConstIterator;

		typedef typename Graph::Edge_Iterator
			Transition_Iterator;
		typedef typename Graph::Edge_ConstIterator
			Transition_ConstIterator;

		class Transition
		{
		public:
			Transition(
				State_ConstIterator state_,
				Symbol symbol_)
				: state(state_)
				, symbol(symbol_)
			{
			}
			
			bool operator==(const Transition& that) const
			{
				return state == that.state &&
					symbol == that.symbol;
			}

			bool operator!=(const Transition& that) const
			{
				return !(*this == that);
			}

			State_ConstIterator state;
			Symbol symbol;
		};

		class Transition_Hash
		{
		public:
			hash_integer operator()(
				const Transition& transition) const
			{
				return combineHash(
					computeHash(&*transition.state),
					computeHash(transition.symbol));
			}
		};

		//! The transitions between states.
		/*!
		Each transition is a triple (A, s, B), where
		A and B are states and s is a symbol. The 
		semantics is that reading the symbol s in state A 
		causes a transition to state B. 
		*/
		typedef std::unordered_map<
			Transition, Transition_Iterator, Transition_Hash> 
			TransitionSet;
	};

}

#endif
