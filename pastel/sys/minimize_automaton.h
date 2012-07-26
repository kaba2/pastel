#ifndef PASTEL_MINIMIZE_AUTOMATON_H
#define PASTEL_MINIMIZE_AUTOMATON_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Removes states which can not be reached from the start state.
	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnreachable(
		Automaton<State, Symbol> automaton)
	{
		return automaton;
	}

	// Removes states which can not reach any final state.
	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnaccepting(
		Automaton<State, Symbol> automaton)
	{
		typedef Automaton<State, Symbol>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		std::unordered_set<State_ConstIterator>
			visitedSet;
		std::vector<State_ConstIterator>
			workSet;
		while(!workSet.empty())
		{
			State_ConstIterator state =
				workSet.back();
			workSet.pop_back();
			

		}

		return automaton;
	}

	template <typename State, typename Symbol>
	Automaton<State, Symbol> minimize(
		Automaton<State, Symbol> automaton)
	{
		return automaton;
	}

}

#endif
