#ifndef PASTELSYS_AUTOMATON_CLOSURE_CONCEPTS_H
#define PASTELSYS_AUTOMATON_CLOSURE_CONCEPTS_H

#include "pastel/sys/automaton/automaton_closure.h"

#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/automaton/automaton_fwd.h"
#include "pastel/sys/optional/optional.h"

namespace Pastel
{

	template <
		typename StateSet,
		typename Symbol,
		typename StateData,
		typename TransitionData>
	class Automaton_Closure_Concepts
	{
	public:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		class ForEachRelated
		{
		public:
			//! Visits each state in the direct closure of 'state'.
			bool operator()(
				const State_ConstIterator& state,
				const std::function<void(const State_ConstIterator&)>& visit) const;
		};

		class Closure_Output
		{
		public:
			//! Reports the closure of a state.
			/*!
			Note that the StateSet-type is explicitly specified
			by the user in the call to automatonClosure().
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
