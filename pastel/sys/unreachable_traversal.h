// Description: Removal of unreachable states

#ifndef PASTEL_UNREACHABLE_TRAVERSAL_H
#define PASTEL_UNREACHABLE_TRAVERSAL_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	//! Removes states which can not be reached from the start state.
	template <
		typename State, 
		typename Symbol,
		typename State_Reporter>
	void forEachUnreachable(
		const Automaton<State, Symbol>& automaton,
		const State_Reporter& report);

}

#include "pastel/sys/unreachable_traversal.hpp"

#endif
