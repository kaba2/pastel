#ifndef PASTELSYS_REACHABLE_STATES_HPP
#define PASTELSYS_REACHABLE_STATES_HPP

#include "pastel/sys/automaton/reachable_states.h"
#include "pastel/sys/graph/depth_first/depth_first.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Mark,
		typename Marked>
	void forEachReachable(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Mark& mark,
		const Marked& marked)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
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
			for (auto incidence = vertex->cOutgoingBegin();
				incidence != vertex->cOutgoingEnd();
				++incidence)
			{
				visit(incidence->vertex());
			}
		};

		traverseGraph<State_ConstIterator>(
			forEachSeedVertex, forEachAdjacent,
			mark, marked,
			DepthFirst_GraphTraversal());
	}

}

#endif
