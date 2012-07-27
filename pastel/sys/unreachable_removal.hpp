#ifndef PASTEL_UNREACHABLE_REMOVAL_HPP
#define PASTEL_UNREACHABLE_REMOVAL_HPP

#include "pastel/sys/unreachable_removal.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnreachable(
		Automaton<State, Symbol> automaton)
	{
		return automaton;
	}

}

#endif
