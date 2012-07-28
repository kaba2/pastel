// Description: Automaton minimization
// Documentation: automaton_algorithms.txt

#ifndef PASTEL_AUTOMATON_MINIMIZATION_H
#define PASTEL_AUTOMATON_MINIMIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Minimizes the given automaton.
	/*!
	Time complexity:
	O(n + m log(n)) in practice, since we use a hash-table to 
	sort transitions by symbols.
	*/
	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> minimizeAutomaton(
		Automaton<Symbol, StateData, TransitionData> automaton);

}

#include "pastel/sys/automaton_minimization.hpp"

#endif
