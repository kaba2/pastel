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
	template <typename State, typename Symbol>
	Automaton<State, Symbol> minimize(
		Automaton<State, Symbol> automaton);

}

#endif
