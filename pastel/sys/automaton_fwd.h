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
		//! The user-data for the states.
		typedef typename Forward<StateData>::type
			StateData_Class;

		//! The user-data for the transitions.
		typedef typename Forward<TransitionData>::type
			TransitionData_Class;

		//! The symbol-labeling of the edge.
		class Label;

		//! The underlying graph.
		typedef Incidence_Graph<GraphType::Directed, 
			StateData, Label> Graph;

		//! The states.
		/*!
		The states are the vertices of the graph.
		*/
		typedef typename Graph::Vertex_Iterator
			State_Iterator;
		typedef typename Graph::Vertex_ConstIterator
			State_ConstIterator;

		//! The transitions.
		/*!
		The transitions are the edges of the graph, 
		labeled with symbols.
		*/
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

		//! A branch-set.
		/*!
		A branch-set is a set of transitions such that the transitions 
		have the same from-state and symbol. These transitions are
		called branches (as in the branching of computation in a 
		non-deterministic automaton). For a deterministic automaton
		the size of a branch set is at most one. This data structure 
		has two responbilities. The first is to store a branch-set
		so that a non-deterministic automaton can be simulated 
		efficiently. The second is to efficiently check whether a 
		transition from a state to another with a given symbol already 
		exists; while the SearchSet checks the from-state-symbol pair,
		the to-state is checked by the BranchSet. This check is needed 
		to enforce the definition of a finite state automaton when 
		inserting new transitions.
		*/
		typedef std::unordered_map<State_ConstIterator, 
			Transition_ConstIterator, IteratorAddress_Hash> BranchSet;
		typedef typename BranchSet::iterator
			Branch_Iterator;
		typedef typename BranchSet::const_iterator
			Branch_ConstIterator;

		//! The branch-set search-set.
		/*!
		This data structure allows to efficiently find the branch-set
		of a given from-state-symbol pair. This is needed to simulate
		any automaton.
		*/
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
