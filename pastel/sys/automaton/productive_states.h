// Description: Traversal of productive automaton states

#ifndef PASTELSYS_PRODUCTIVE_STATES_H
#define PASTELSYS_PRODUCTIVE_STATES_H

#include "pastel/sys/automaton.h"

#include "pastel/sys/automaton/productive_states_concepts.h"

namespace Pastel
{

	//! Traverses those states which can reach a final state.
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Mark,
		typename Marked>
	void forEachProductive(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Mark& mark,
		const Marked& marked);

}

#include "pastel/sys/automaton/productive_states.hpp"

#endif
