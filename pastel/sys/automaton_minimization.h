// Description: Automaton minimization
// Documentation: automaton_algorithms.txt

#ifndef PASTEL_AUTOMATON_MINIMIZATION_H
#define PASTEL_AUTOMATON_MINIMIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol> minimize(
		Automaton<State, Symbol> automaton)
	{
		return automaton;
	}

}

#endif
