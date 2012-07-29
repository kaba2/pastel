// Description: Removal of unreachable states

#ifndef PASTEL_REACHABLE_STATES_H
#define PASTEL_REACHABLE_STATES_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Reports the states which can be reached from the start state.
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Mark,
		typename Marked>
	void forEachReachable(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		const Mark& mark,
		const Marked& marked);

}

#include "pastel/sys/reachable_states.hpp"

#endif
