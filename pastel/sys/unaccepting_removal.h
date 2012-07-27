// Description: Removal of unaccepting states

#ifndef PASTEL_UNACCEPTING_REMOVAL_H
#define PASTEL_UNACCEPTING_REMOVAL_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	// Removes states which can not reach any final state.
	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnaccepting(
		Automaton<State, Symbol> automaton);

}

#include "pastel/sys/unaccepting_removal.hpp"

#endif
