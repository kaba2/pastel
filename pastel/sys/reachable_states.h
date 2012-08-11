// Description: Traversal of reachable automaton states

#ifndef PASTEL_REACHABLE_STATES_H
#define PASTEL_REACHABLE_STATES_H

#include "pastel/sys/reachable_states_concepts.h"

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Traverses the states which can be reached from the start state.
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Mark,
		typename Marked>
	void forEachReachable(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Mark& mark,
		const Marked& marked);

}

#include "pastel/sys/reachable_states.hpp"

#endif
