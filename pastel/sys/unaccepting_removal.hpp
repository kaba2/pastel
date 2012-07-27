#ifndef PASTEL_UNACCEPTING_REMOVAL_HPP
#define PASTEL_UNACCEPTING_REMOVAL_HPP

#include "pastel/sys/unaccepting_removal.h"
#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol> removeUnaccepting(
		Automaton<State, Symbol> automaton)
	{
		if (automaton.finalStates() == 0)
		{
			// Since the automaton does not have any
			// final states, every state is unaccepting.

			automaton.clear();
		}

		typedef Automaton<State, Symbol>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		auto forEachSeedVertex = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			// Insert final states.
		};

		auto forEachAdjacent =
			[&](const State_ConstIterator& vertex,
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto edge = vertex->incomingBegin();
				edge != vertex->incomingEnd();
				++edge)
			{
				visit(edge->vertex());
			}
		};

		// See which vertices can reach a final state.
		TraverseDepthFirst<State_ConstIterator, IteratorAddress_Hash> traversal(
			forEachSeedVertex, forEachAdjacent, [](){});
		
		// Remove those vertices which were not visited.
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			if (!traversal.visitedSet.count(state))
			{
				automaton.erase(state);
			}
		}

		return automaton;
	}

}

#endif
