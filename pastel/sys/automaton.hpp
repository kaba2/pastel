#ifndef PASTEL_AUTOMATON_HPP
#define PASTEL_AUTOMATON_HPP

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	auto transition(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		typename Automaton<Symbol, StateData, TransitionData>::State_ConstIterator state,
		Symbol symbol)
		-> typename Automaton<Symbol, StateData, TransitionData>::State_ConstIterator
	{
		auto transition = automaton.findTransition(
			Automaton<Symbol, StateData, TransitionData>::StateSymbol(state, symbol));
		if (transition != automaton.cTransitionEnd())
		{
			// There is such a transition. 
			// Return the target state.
			return transition->to();
		}
		
		// There is no such transition. 
		// Return the reject state.
		return automaton.rejectState();
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Symbol_ConstRange>
	auto transition(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		typename Automaton<Symbol, StateData, TransitionData>::State_ConstIterator state,
		Symbol_ConstRange symbolSet)
		-> typename Automaton<Symbol, StateData, TransitionData>::State_ConstIterator
	{
		for (auto symbol = symbolSet.begin();
			symbol != symbolSet.end();
			++symbol)
		{
			state = transition(automaton, state, *symbol);
			if (state == automaton.rejectState())
			{
				// There was no such transition;
				// we are now in the reject state. Since
				// there are no transitions away from
				// the reject state, we can stop now.
				break;
			}
		}

		return state;
	}

}

#endif
