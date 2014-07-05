#ifndef PASTELSYS_AUTOMATON_FWD_H
#define PASTELSYS_AUTOMATON_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/class.h"
#include "pastel/sys/optional.h"
#include "pastel/sys/second_iterator.h"
#include "pastel/sys/hashing.h"

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
		/*
		The type-definitions below need to be given in terms
		of Graph_Fwd, rather than Graph, because of a bug in 
		gcc 4.6.
		*/
		typedef Directed_Graph<StateLabel, TransitionLabel> 
			Graph;
		typedef IncidenceGraph_Fwd<typename Graph::Settings> 
			Graph_Fwd;

		//! The states.
		/*!
		The states are the vertices of the graph.
		*/
		typedef typename Graph_Fwd::Vertex_Iterator
			State_Iterator;
		typedef typename Graph_Fwd::Vertex_ConstIterator
			State_ConstIterator;

		//! The user-data for the states.
		using StateData_Class = As_Class<StateData>
;

		//! The transitions.
		/*!
		The transitions are the edges of the graph, 
		labeled with symbols.
		*/
		typedef typename Graph_Fwd::Edge_Iterator
			Transition_Iterator;
		typedef typename Graph_Fwd::Edge_ConstIterator
			Transition_ConstIterator;

		//! The user-data for the transitions.
		using TransitionData_Class = As_Class<TransitionData>
;

		typedef typename Graph_Fwd::Incidence_Iterator
			Incidence_Iterator;
		typedef typename Graph_Fwd::Incidence_ConstIterator
			Incidence_ConstIterator;

		//! The set of final states.
		using FinalSet = std::list<State_ConstIterator>
;
		typedef typename FinalSet::iterator
			Final_Iterator;
	#ifdef __GNUC__
		/*
		Since even gcc 4.7 does not implement the C++11 
		container support for const_iterators properly,
		we will have to use mutable iterators as a hack.
		*/
		using Final_ConstIterator = Final_Iterator
;
	#else
		typedef typename FinalSet::const_iterator
			Final_ConstIterator;
	#endif

		//! The set of start states.
		using StartSet = std::list<State_ConstIterator>
;
		typedef typename StartSet::iterator
			Start_Iterator;
	#ifdef __GNUC__
		using Start_ConstIterator = Start_Iterator
;
	#else
		typedef typename StartSet::const_iterator
			Start_ConstIterator;
	#endif

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
	#ifdef __GNUC__
		using Actual_Branch_ConstIterator = Actual_Branch_Iterator
;
	#else
		typedef typename BranchMap::const_iterator
			Actual_Branch_ConstIterator;
	#endif
		typedef Second_Iterator<Actual_Branch_Iterator, false>
			Branch_Iterator;
	#ifdef __GNUC__
		using Branch_ConstIterator = Branch_Iterator
;
	#else
		typedef Second_Iterator<Actual_Branch_ConstIterator, true>
			Branch_ConstIterator;
	#endif
	
	private:
		template <
			typename, typename, 
			typename, typename>
		friend class Automaton;

		class StateSymbol
		{
		public:
			StateSymbol(
				State_ConstIterator state_,
				Optional<Symbol> symbol_)
				: state(std::move(state_))
				, symbol(std::move(symbol_))
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
