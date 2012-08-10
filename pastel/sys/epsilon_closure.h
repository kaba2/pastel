#ifndef PASTEL_EPSILON_CLOSURE_H
#define PASTEL_EPSILON_CLOSURE_H

#include "pastel/sys/epsilon_closure_concepts.h"

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Computes the epsilon-closure of each state.
	/*!
	The epsilon-closure of a state A is the set of those states which 
	are	reachable by epsilon-transitions from A. The type of the 
	state-set to contain these states needs to be explicitly specified
	by the user. This type needs to be default-constructible to an
	empty set. Since the algorithm does not know the type of the
	state-set, the user must also provide a way to insert a state
	into the state-set by the 'insert' functional. The epsilon-closures
	are reported by the 'report' functional. Have a look at 
	epsilon_closure_concepts.h for more information.
	*/
	template <
		typename StateSet,
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Closure_Reporter,
		typename Insert_State>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Insert_State& insert,
		const Closure_Reporter& report);

}

#include "pastel/sys/epsilon_closure.hpp"

#endif
