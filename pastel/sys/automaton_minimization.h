// Description: Automaton minimization
// Documentation: automaton_algorithms.txt

#ifndef PASTEL_AUTOMATON_MINIMIZATION_H
#define PASTEL_AUTOMATON_MINIMIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Minimizes the given automaton.
	/*!
	Preconditions:
	The automaton is deterministic; it has at most one start state,
	and a transition from a state q with a symbol s always leads to
	the same state p (this definition allows multiple such transitions).

	Time complexity:
	O(n + m log(n)) in practice (*), where n is the number of
	states, and m is the number of transitions.
	
	(*) Since we use a hash-table to sort transitions by symbols.
	*/
	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> minimizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData>& automaton);

}

#include "pastel/sys/automaton_minimization.hpp"

#endif
