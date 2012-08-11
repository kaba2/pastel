// Description: Minimization of a deterministic automaton
// Documentation: automaton_algorithms.txt

#ifndef PASTEL_AUTOMATON_MINIMIZATION_H
#define PASTEL_AUTOMATON_MINIMIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Minimizes the given deterministic automaton.
	/*!
	Preconditions:
	The automaton is deterministic; it has at most one start state,
	there is at most one transition between states and q and p with 
	a given symbol s, and there are no epsilon-transitions.

	Time complexity:
	O(n + m log(n)) in practice (*), where n is the number of
	states, and m is the number of transitions.
	
	(*) Since hash-tables are used, this can't be guaranteed in the 
	worst case, as always.
	*/
	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization>
	Automaton<Symbol, StateData, TransitionData, Customization> minimizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton);

}

#include "pastel/sys/automaton_minimization.hpp"

#endif
