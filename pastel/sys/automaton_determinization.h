// Description: Automaton determinization

#ifndef PASTEL_AUTOMATON_DETERMINIZATION_H
#define PASTEL_AUTOMATON_DETERMINIZATION_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Determinizes the given automaton.
	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> determinizeAutomaton(
		Automaton<Symbol, StateData, TransitionData> automaton);

}

#include "pastel/sys/automaton_determinization.hpp"

#endif
