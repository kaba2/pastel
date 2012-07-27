// Description: Removal of unreachable states

#ifndef PASTEL_UNREACHABLE_REMOVAL_H
#define PASTEL_UNREACHABLE_REMOVAL_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Removes states which can not be reached from the start state.
	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnreachable(
		Automaton<State, Symbol> automaton);

}

#include "pastel/sys/unreachable_removal.hpp"

#endif
