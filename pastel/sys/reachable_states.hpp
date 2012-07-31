#ifndef PASTEL_REACHABLE_STATES_HPP
#define PASTEL_REACHABLE_STATES_HPP

#include "pastel/sys/reachable_states.h"
#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Mark,
		typename Marked>
	void forEachReachable(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		const Mark& mark,
		const Marked& marked)
	{
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		auto forEachSeedVertex = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			std::for_each(
				automaton.cStartBegin(),
				automaton.cStartEnd(),
				visit);
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

		traverseGraph<State_ConstIterator>(
			forEachSeedVertex, forEachAdjacent,
			mark, marked,
			DepthFirst_GraphTraversal());
	}

}

#endif
