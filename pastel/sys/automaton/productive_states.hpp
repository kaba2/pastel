#ifndef PASTELSYS_PRODUCTIVE_STATES_HPP
#define PASTELSYS_PRODUCTIVE_STATES_HPP

#include "pastel/sys/automaton/productive_states.h"

#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Mark,
		typename Marked>
	void forEachProductive(
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
			for (auto incidence = vertex->cIncomingBegin();
				incidence != vertex->cIncomingEnd();
				++incidence)
			{
				visit(incidence->vertex());
			}
		};

		traverseGraph<State_ConstIterator>(
			forEachSeedVertex, forEachAdjacent, 
			mark, marked, DepthFirst_GraphTraversal());
	}

}

#endif
