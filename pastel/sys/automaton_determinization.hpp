#ifndef PASTEL_AUTOMATON_DETERMINIZATION_H
#define PASTEL_AUTOMATON_DETERMINIZATION_H

#include "pastel/sys/automaton_determinization.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> determinizeAutomaton(
		Automaton<Symbol, StateData, TransitionData> automaton)
	{
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		typedef std::unordered_set<State_ConstIterator>
			StateSet;
		

	}

}

#endif
