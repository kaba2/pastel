#ifndef PASTELSYS_AUTOMATON_SIMULATION_HPP
#define PASTELSYS_AUTOMATON_SIMULATION_HPP

#include "pastel/sys/automaton_simulation.h"

#include <algorithm>

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
		const Insert_State& insert)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization> 
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		StateSet toStateSet(emptySet);

		// Find out the state-sets which are produced
		// by following the transitions with the given symbol
		// for each state.
		std::for_each(
			stateSet.cbegin(), stateSet.cend(),
			[&](const State_ConstIterator& state)
		{
			auto branchRange = automaton.cBranchRange(
				state, symbol);

			std::for_each(
				branchRange.begin(),
				branchRange.end(),
				[&](const Transition_ConstIterator& transition)
			{
				insert(transition->to(), toStateSet);
			});
		});

		return toStateSet;
	}

}

#endif
