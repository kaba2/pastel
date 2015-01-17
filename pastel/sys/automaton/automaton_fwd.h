#ifndef PASTELSYS_AUTOMATON_FWD_H
#define PASTELSYS_AUTOMATON_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/incidence_graph/incidence_graph_fwd.h"
#include "pastel/sys/generic/class.h"
#include "pastel/sys/optional.h"
#include "pastel/sys/iterator/second_iterator.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"
#include "pastel/sys/hashing.h"

#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization>
	class Automaton;

	namespace Automaton_
	{

		template <
			typename Symbol,
			typename StateData,
			typename TransitionData>
		class StateLabel;

		template <
			typename Symbol, 
			typename StateData, 
			typename TransitionData>
		class TransitionLabel;

	}

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class Automaton_Fwd
	{
	public:
		//! The start/final-labeling of the state.
		using StateLabel = 
			Automaton_::StateLabel<Symbol, StateData, TransitionData>;

		//! The symbol-labeling of the transition.
		using TransitionLabel = 
			Automaton_::TransitionLabel<Symbol, StateData, TransitionData>;

		//! The underlying graph.
		using Graph_Settings = IncidenceGraph_Settings<
			GraphType::Directed, 
			StateLabel, 
			TransitionLabel>;
		typedef IncidenceGraph_Fwd<Graph_Settings> 
			Graph_Fwd;
		typedef IncidenceGraph<Graph_Settings> 
			Graph;

		//! The states.
		/*!
		The states are the vertices of the graph.
		*/
		typedef typename Graph_Fwd::Vertex_Iterator
			State_Iterator;
		typedef typename Graph_Fwd::Vertex_ConstIterator
			State_ConstIterator;

		//! The user-data for the states.
		struct State_Tag;
		using StateData_Class = 
			typename As_Class<StateData, State_Tag>::type;
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
		struct TransitionData_Tag;
		using TransitionData_Class = 
			typename As_Class<TransitionData, TransitionData_Tag>::type;

		typedef typename Graph_Fwd::Incidence_Iterator
			Incidence_Iterator;
		typedef typename Graph_Fwd::Incidence_ConstIterator
			Incidence_ConstIterator;

		//! The set of final states.
		using FinalSet = std::list<State_ConstIterator>;

		typedef typename FinalSet::iterator
			Final_Iterator;
	#ifdef __GNUC__
		/*
		Since even gcc 4.7 does not implement the C++11 
		container support for const_iterators properly,
		we will have to use mutable iterators as a hack.
		*/
		using Final_ConstIterator = Final_Iterator;
	#else
		typedef typename FinalSet::const_iterator
			Final_ConstIterator;
	#endif

		//! The set of start states.
		using StartSet = std::list<State_ConstIterator>;
		typedef typename StartSet::iterator
			Start_Iterator;
	#ifdef __GNUC__
		using Start_ConstIterator = Start_Iterator;
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
		using Actual_Branch_ConstIterator = Actual_Branch_Iterator;
	#else
		typedef typename BranchMap::const_iterator
			Actual_Branch_ConstIterator;
	#endif
		typedef Second_Iterator<Actual_Branch_Iterator, false>
			Branch_Iterator;
	#ifdef __GNUC__
		using Branch_ConstIterator = Branch_Iterator;
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

#include "pastel/sys/automaton/automaton_state_label.h"
#include "pastel/sys/automaton/automaton_transition_label.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData>
	class No_Automaton_Customization
	{
	private:
		using Fwd = Automaton_Fwd<Symbol, StateData, TransitionData>;
	
		PASTEL_FWD(State_Iterator);
		PASTEL_FWD(State_ConstIterator);
		PASTEL_FWD(Transition_Iterator);
		PASTEL_FWD(Transition_ConstIterator);

	protected:
		No_Automaton_Customization() {}
		No_Automaton_Customization(const No_Automaton_Customization& that) {}
		No_Automaton_Customization(No_Automaton_Customization&& that) {}
		No_Automaton_Customization& operator=(
			No_Automaton_Customization that) {return *this;}

		void swap(No_Automaton_Customization& that) {}

		void onClear() {}
		void onClearTransitions() {}
		void onClearStart() {}
		void onClearFinal() {}
		
		void onAddState(const State_Iterator& state) {}
		void onRemoveState(const State_ConstIterator& state) {}
		
		void onAddStart(
			const State_ConstIterator& state) {}
		void onRemoveStart(
			const State_ConstIterator& state) {}
		
		void onAddFinal(
			const State_ConstIterator& state) {}
		void onRemoveFinal(
			const State_ConstIterator& state) {}
		
		void onAddTransition(
			const Transition_ConstIterator& transition)	{}
		void onRemoveTransition(
			const Transition_ConstIterator& transition)	{}
		
		void onMerge(Automaton<Symbol, StateData, TransitionData,
			No_Automaton_Customization>& that) {}
		
		bool canAddTransition(
			const State_ConstIterator& fromState,
			const Optional<Symbol>& symbol,
			const State_ConstIterator& toState) 
		{
			return true;
		}
	};

}

#endif
