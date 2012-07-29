// Description: Traversal of productive states

#ifndef PASTEL_PRODUCTIVE_STATES_H
#define PASTEL_PRODUCTIVE_STATES_H

#include "pastel/sys/automaton.h"

#include "pastel/sys/productive_states_concepts.h"

namespace Pastel
{

	//! Traverses those states which can reach a final state.
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Mark,
		typename Marked>
	void forEachProductive(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		const Mark& mark,
		const Marked& marked);

}

#include "pastel/sys/productive_states.hpp"

#endif
