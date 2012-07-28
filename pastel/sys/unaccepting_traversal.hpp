#ifndef PASTEL_UNACCEPTING_TRAVERSAL_HPP
#define PASTEL_UNACCEPTING_TRAVERSAL_HPP

#include "pastel/sys/unaccepting_traversal.h"
#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <typename State, typename Symbol, 
		typename State_Reporter>
	void forEachUnaccepting(
		const Automaton<State, Symbol>& automaton,
		const State_Reporter& report)
	{
		if (automaton.finalStates() == automaton.states())
		{
			// Every state is a final state.
			return;
		}

		if (automaton.finalStates() == 0)
		{
			// Every state is unaccepting.
			for (auto state = automaton.cStateBegin();
				state != automaton.cStateEnd();
				++state)
			{
				report(state);
			}

			return;
		}

		typedef Automaton<State, Symbol>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		auto forEachSeedVertex = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto state = automaton.cFinalBegin();
				state != automaton.cFinalEnd();
				++state)
			{
				visit(*state);
			}
		};

		auto forEachAdjacent =
			[&](const State_ConstIterator& vertex,
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto edge = vertex->cIncomingBegin();
				edge != vertex->cIncomingEnd();
				++edge)
			{
				visit(edge->vertex());
			}
		};

		// See which vertices can reach a final state.
		TraverseDepthFirst<State_ConstIterator, IteratorAddress_Hash> traversal(
			forEachSeedVertex, forEachAdjacent, [](const State_ConstIterator&){});
		
		// Report those vertices which were not visited.
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			if (!traversal.visitedSet.count(state))
			{
				report(state);
			}
		}
	}

}

#endif
