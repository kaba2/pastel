#ifndef PASTEL_AUTOMATON_FWD_H
#define PASTEL_AUTOMATON_FWD_H

#include "pastel/sys/hash.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/object_forwarding.h"

#include <list>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData = void, 
		typename TransitionData = void>
	class Automaton_Fwd
	{
	public:
		typedef typename Forward<StateData>::type
			StateData_Class;
		typedef typename Forward<TransitionData>::type
			TransitionData_Class;

		class Transition;

		typedef Incidence_Graph<GraphType::Directed, StateData, Transition> Graph;

		typedef typename Graph::Vertex_Iterator
			State_Iterator;
		typedef typename Graph::Vertex_ConstIterator
			State_ConstIterator;

		typedef typename Graph::Edge_Iterator
			Transition_Iterator;
		typedef typename Graph::Edge_ConstIterator
			Transition_ConstIterator;

		class StateSymbol
		{
		public:
			StateSymbol(
				State_ConstIterator state_,
				Symbol symbol_)
				: state(state_)
				, symbol(symbol_)
			{
			}
			
			bool operator==(const StateSymbol& that) const
			{
				return state == that.state &&
					symbol == that.symbol;
			}

			bool operator!=(const StateSymbol& that) const
			{
				return !(*this == that);
			}

			State_ConstIterator state;
			Symbol symbol;
		};

		class StateSymbol_Hash
		{
		public:
			hash_integer operator()(
				const StateSymbol& transition) const
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
			StateSymbol, Transition_Iterator, StateSymbol_Hash> 
			SearchSet;
	};

}

#endif
