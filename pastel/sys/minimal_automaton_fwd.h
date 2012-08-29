#ifndef PASTELSYS_MINIMAL_AUTOMATON_FWD_H
#define PASTELSYS_MINIMAL_AUTOMATON_FWD_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class MinimalAutomaton_Fwd
	{
	public:
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;

		typedef typename Automaton::State_Iterator
			State_Iterator;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		typedef typename Automaton::Transition_Iterator
			Transition_Iterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;
	};

}

#endif
