#ifndef PASTEL_UNREACHABLE_REMOVAL_HPP
#define PASTEL_UNREACHABLE_REMOVAL_HPP

#include "pastel/sys/unreachable_removal.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnreachable(
		Automaton<State, Symbol> automaton)
	{
		if (automaton.startState() == automaton.rejectState())
		{
			// Since there is no start state, no state
			// can be reached.

			automaton.clear();
		}

		return automaton;
	}

}

#endif
