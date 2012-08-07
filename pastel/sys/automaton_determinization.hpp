#ifndef PASTEL_AUTOMATON_DETERMINIZATION_H
#define PASTEL_AUTOMATON_DETERMINIZATION_H

#include "pastel/sys/automaton_determinization.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class DeterminizeAutomaton
	{
	private:
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		class StateSet
		{
		public:

			std::set<State_ConstIterator> stateSet_;
			hash_integer hash_;
		};
	};

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> determinizeAutomaton(
		Automaton<Symbol, StateData, TransitionData> automaton)
	{


		typedef std::unordered_set<State_ConstIterator>
			StateSet;
		

	}

}

#endif
