#ifndef PASTEL_UNREACHABLE_TRAVERSAL_HPP
#define PASTEL_UNREACHABLE_TRAVERSAL_HPP

#include "pastel/sys/unreachable_traversal.h"
#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <
		typename State, 
		typename Symbol,
		typename State_Reporter>
	void forEachUnreachable(
		const Automaton<State, Symbol>& automaton,
		const State_Reporter& report)
	{
		typedef Automaton<State, Symbol>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		auto forEachSeedVertex = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			visit(automaton.startState());
		};

		auto forEachAdjacent =
			[&](const State_ConstIterator& vertex,
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto edge = vertex->cOutgoingBegin();
				edge != vertex->cOutgoingEnd();
				++edge)
			{
				visit(edge->vertex());
			}
		};

		// See which vertices can be reached from the start state.
		TraverseDepthFirst<State_ConstIterator, IteratorAddress_Hash> traversal(
			forEachSeedVertex, forEachAdjacent, [](const State_ConstIterator&){});

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
