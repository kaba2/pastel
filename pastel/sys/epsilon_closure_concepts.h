#ifndef PASTEL_EPSILON_CLOSURE_CONCEPTS_H
#define PASTEL_EPSILON_CLOSURE_CONCEPTS_H

#include "pastel/sys/epsilon_closure.h"

#include "pastel/sys/automaton_fwd.h"

namespace Pastel
{

	template <
		typename StateSet,
		typename Symbol,
		typename StateData,
		typename TransitionData>
	class Epsilon_Closure_Concepts
	{
	public:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		class Closure_Reporter
		{
		public:
			//! Reports the epsilon-closure of a state.
			/*!
			Note that the StateSet-type is explicitly specified
			by the user in the call to epsilonClosure().
			*/
			void operator()(
				const State_ConstIterator& state,
				StateSet&& stateSet) const;
		};

		class Insert_State
		{
		public:
			//! Inserts a state into a state-set.
			void operator()(
				const State_ConstIterator& state,
				StateSet& stateSet) const;
		};
	};

}

#endif
