// Description: Automaton determinization

#ifndef PASTELSYS_AUTOMATON_DETERMINIZATION_H
#define PASTELSYS_AUTOMATON_DETERMINIZATION_H

#include "pastel/sys/automaton_determinization_concepts.h"

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Determinizes the given automaton.
	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization,
		typename State_Output,
		typename Transition_Output>
	void determinizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const State_Output& reportState,
		const Transition_Output& reportTransition);

}

#include "pastel/sys/automaton_determinization.hpp"

#endif
