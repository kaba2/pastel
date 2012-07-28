// Description: Removal of unaccepting states

#ifndef PASTEL_UNACCEPTING_TRAVERSAL_H
#define PASTEL_UNACCEPTING_TRAVERSAL_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Traverses states which can not reach any final state.
	template <typename State, typename Symbol, 
		typename State_Reporter>
	void forEachUnaccepting(
		const Automaton<State, Symbol>& automaton,
		const State_Reporter& report);

}

#include "pastel/sys/unaccepting_traversal.hpp"

#endif
