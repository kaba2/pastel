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

		//! A set of transitions.
		typedef std::unordered_map<State_ConstIterator, 
			Transition_ConstIterator, IteratorAddress_Hash> BranchSet;
		typedef typename BranchSet::iterator
			Branch_Iterator;
		typedef typename BranchSet::const_iterator
			Branch_ConstIterator;

		//! The transition search set.
		typedef std::unordered_map<
			StateSymbol, BranchSet, StateSymbol_Hash> 
			SearchSet;
		typedef typename SearchSet::iterator
			Search_Iterator;
		typedef typename SearchSet::const_iterator
			Search_ConstIterator;

		//! The set of final states.
		typedef std::unordered_set<
			State_ConstIterator, IteratorAddress_Hash>
			FinalSet;
		typedef typename FinalSet::iterator
			Final_Iterator;
		typedef typename FinalSet::const_iterator
			Final_ConstIterator;
	};

}

#endif
