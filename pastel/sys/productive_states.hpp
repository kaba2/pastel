#ifndef PASTEL_PRODUCTIVE_STATES_HPP
#define PASTEL_PRODUCTIVE_STATES_HPP

#include "pastel/sys/productive_states.h"

#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Mark,
		typename Marked>
	void forEachProductive(
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

		traverseDepthFirst<State_ConstIterator>(
			forEachSeedVertex, forEachAdjacent, 
			mark, marked);
	}

}

#endif
