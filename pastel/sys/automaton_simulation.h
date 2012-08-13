// Description: Simulation of an automaton

#ifndef PASTEL_AUTOMATON_SIMULATION_H
#define PASTEL_AUTOMATON_SIMULATION_H

//#include "pastel/sys/automaton_simulation_concepts.h"

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename StateSet,
		typename Insert_State>
	StateSet simulateAutomatonWithoutEpsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Optional<Symbol>& symbol,
		const StateSet& emptySet,
		const StateSet& stateSet,
		const Insert_State& insert);

}

#include "pastel/sys/automaton_simulation.hpp"

#endif
