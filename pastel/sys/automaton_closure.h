// Description: Closure of automaton states

#ifndef PASTELSYS_AUTOMATON_CLOSURE_H
#define PASTELSYS_AUTOMATON_CLOSURE_H

#include "pastel/sys/automaton_closure_concepts.h"

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Computes the closure of each state in the automaton.
	/*!
	The closure of a state A is the set of those states which 
	are	reachable from A by transitions with closure symbols. 
	The type of the state-set to contain these states needs to 
	be explicitly specified by the user. Have a look at 
	automaton_closure_concepts.h for more information.

	Preconditions:
	StateSet is copy-constructible.

	forEachRelated:
	Visits each state in the direct closure of 'state'.

	insert:
	Inserts the given state in the given state-set.

	report:
	Reports the closure-set of a specific state.
	*/
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename StateSet,
		typename ForEachRelated,
		typename Insert_State,
		typename Closure_Reporter>
	void automatonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const StateSet& emptySet,
		const ForEachRelated& forEachRelated,
		const Insert_State& insert,
		const Closure_Reporter& report);

	//! Compures the epsilon-closure of each state in the automaton.
	/*!
	This is a convenience function which calls automatonClosure()
	with the ForEachRelated traversing the epsilon-transitions.
	*/
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename StateSet,
		typename Insert_State,
		typename Closure_Reporter>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const StateSet& emptySet,
		const Insert_State& insert,
		const Closure_Reporter& report);
}

#include "pastel/sys/automaton_closure.hpp"

#endif
