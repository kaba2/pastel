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
		typename TransitionData,
		typename Customization,
		typename State_Reporter,
		typename Transition_Reporter>
	void determinizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Symbol& epsilon,
		const State_Reporter& reportState,
		const Transition_Reporter& transitionReport);

}

#include "pastel/sys/automaton_determinization.hpp"

#endif
