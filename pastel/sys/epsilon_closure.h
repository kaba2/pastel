#ifndef PASTEL_EPSILON_CLOSURE_H
#define PASTEL_EPSILON_CLOSURE_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization
		typename State_StateSet_Reporter>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Symbol& epsilon,
		const State_StateSet_Reporter& report);

}

#include "pastel/sys/epsilon_closure.hpp"

#endif
