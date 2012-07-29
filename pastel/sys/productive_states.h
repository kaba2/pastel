// Description: Removal of unaccepting states

#ifndef PASTEL_PRODUCTIVE_STATES_H
#define PASTEL_PRODUCTIVE_STATES_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Reports the productive states.
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename State_Reporter,
		typename Mark,
		typename Marked>
	void forEachProductive(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		const State_Reporter& report,
		const Mark& mark,
		const Marked& marked);

}

#include "pastel/sys/productive_states.hpp"

#endif
