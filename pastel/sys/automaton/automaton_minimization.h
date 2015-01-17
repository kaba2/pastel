// Description: Minimization of a deterministic automaton

#ifndef PASTELSYS_AUTOMATON_MINIMIZATION_H
#define PASTELSYS_AUTOMATON_MINIMIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Minimizes the given deterministic automaton.
	/*!
	Preconditions:
	automaton.deterministic()

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

#include "pastel/sys/automaton/automaton_minimization.hpp"

#endif
