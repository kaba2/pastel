#ifndef PASTEL_AUTOMATON_FWD_H
#define PASTEL_AUTOMATON_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/optional.h"
#include "pastel/sys/second_iterator.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization>
	class Automaton;

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

		typedef typename Graph::Incidence_Iterator
			Incidence_Iterator;
		typedef typename Graph::Incidence_ConstIterator
			Incidence_ConstIterator;

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

		//! A set of branches.
		/*!
		A branch-set is a set of transitions with the same
		from-state and a symbol. The elements of the branch-set
		are called branches, referring to how to the computation
		in a non-deterministic automaton branches when there are 
		multiple branches to follow.
		*/
		typedef std::unordered_map<
			State_ConstIterator, Transition_ConstIterator,
			IteratorAddress_Hash>
			BranchMap;
		typedef typename BranchMap::iterator
			Actual_Branch_Iterator;
		typedef typename BranchMap::const_iterator
			Actual_Branch_ConstIterator;
		typedef Second_Iterator<Actual_Branch_Iterator, false>
			Branch_Iterator;
		typedef Second_Iterator<Actual_Branch_ConstIterator, true>
			Branch_ConstIterator;
	
	private:
		template <
			typename Symbol, 
			typename StateData, 
			typename TransitionData,
			typename Customization>
		friend class Automaton;

		class StateSymbol
		{
		public:
			StateSymbol(
				State_ConstIterator state_,
				Optional<Symbol> symbol_)
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
			Optional<Symbol> symbol;
		};

		class StateSymbol_Hash
		{
		public:
			Pastel::hash_integer operator()(
				const StateSymbol& transition) const
			{
				return Pastel::combineHash(
					Pastel::computeHash(&*transition.state),
					Pastel::computeHash(transition.symbol));
			}
		};

		typedef std::unordered_map<StateSymbol, 
			BranchMap, StateSymbol_Hash> Symbol_BranchMap_Map;
		typedef typename Symbol_BranchMap_Map::iterator
			Symbol_BranchMap_Iterator;
		typedef typename Symbol_BranchMap_Map::const_iterator
			Symbol_BranchMap_ConstIterator;
	};

}

#endif
