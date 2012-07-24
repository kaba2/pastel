// Description: Finite-state automaton

#ifndef PASTEL_AUTOMATON_H
#define PASTEL_AUTOMATON_H

#include "pastel/sys/automaton_fwd.h"

namespace Pastel
{

	//! Finite-state automaton
	template <typename State, typename Symbol>
	class Automaton
		: public Automaton_Fwd<State, Symbol>
	{
	public:
		//! Constructs an empty automaton.
		Automaton();

		//! Move-constructs from another automaton.
		Automaton(Automaton&& that);

		//! Move-assigns from another automaton.
		Automaton& operator=(Automaton that);

		//! Removes all states and transitions.
		void clear();

		//! Removes all transitions.
		void clearTransitions();

		//! Swaps two parse-tables.
		void swap(Automaton& that);

		// States

		//! Adds a new state.
		State_Iterator addState(State state = State());

		//! Removes a state.
		void removeState(State_ConstIterator state);

		//! Removes constness from a state-iterator.
		State_Iterator cast(State_ConstIterator state);

		//! Sets the start-state.
		void setStartState(
			State_ConstIterator state);

		//! Returns the start-state.
		State_ConstIterator startState() const;

		//! Returns the reject-state.
		State_ConstIterator rejectState() const;

		//! Returns the first iterator of the state-set.
		State_Iterator stateBegin();

		//! Returns the first iterator of the state-set.
		State_ConstIterator cStateBegin() const;

		//! Returns the end-iterator of the state-set.
		State_Iterator stateEnd();

		//! Returns the end-iterator of the state-set.
		State_ConstIterator cStateEnd() const;

		//! Returns the number of states.
		integer states() const;

		// Transitions

		//! Adds a new transition.
		Transition_ConstIterator addTransition(
			State_ConstIterator fromState,
			Symbol symbol,
			State_ConstIterator toState);

		//! Removes a transition.
		void removeTransition(
			const Transition_ConstIterator& transition);

		//! Searches for the given transition.
		Transition_ConstIterator findTransition(
			const Transition& transition) const;

		//! Tests for the existence of a transition.
		bool existsTransition(
			const Transition& transition) const;

		//! Returns the first iterator of the transition-set.
		Transition_ConstIterator cTransitionBegin() const;

		//! Returns the end-iterator of the transition-set.
		Transition_ConstIterator cTransitionEnd() const;

		//! Returns the first iterator of the transition-set of the state.
		Transition_ConstIterator cTransitionBegin(
			const State_ConstIterator& state) const;

		//! Returns the end-iterator of the transition-set of the state.
		Transition_ConstIterator cTransitionEnd(
			const State_ConstIterator& state) const;

		//! Returns the number of transitions.
		integer transitions() const;

	private:
		// Deleted
		Automaton(const Automaton& that);

		//! Initialization common to all constructors.
		/*!
		This function creates the reject state.
		*/
		void construct();

		//! The underlying graph.
		Graph graph_;
		
		//! Fast associative searches of transitions.
		/*!
		Given a transition (q, X), where q is a state
		and X is a symbol, this multi-map contains all the
		edges from state q with symbol X. There might be
		many such edges if the automaton is non-deterministic.
		*/
		TransitionSet transitionSet_;

		//! Storage for the reject state.
		/*!
		The reject-state is stored here rather than in 
		the 'graph' to make it simpler to preserve the 
		identity of the reject state.
		*/
		Graph rejectGraph_;

		//! The start state.
		State_Iterator startState_;

		//! The reject state.
		/*!
		The reject state ensures that each transition in the automaton 
		always has a dereferencable state. This gets rid of boundary
		cases in the code.
		*/
		State_Iterator rejectState_;
	};

}

namespace Pastel
{

	//! Returns the target-state of a state-symbol transition.
	/*!
	If the transition does not exists, the 
	automaton.rejectState() is returned instead.
	*/
	template <typename State, typename Symbol>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol symbol)
		-> typename Automaton<State, Symbol>::State_ConstIterator;

	//! Returns the target-state of a state-symbol-list transition.
	/*!
	If some of the transitions does not exists, the 
	automaton.rejectState() is returned instead.
	*/
	template <typename State, typename Symbol,
		typename Symbol_ConstRange>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol_ConstRange symbolSet)
		-> typename Automaton<State, Symbol>::State_ConstIterator;

}

#include "pastel/sys/automaton.hpp"

#endif
