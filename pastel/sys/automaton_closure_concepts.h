#ifndef PASTEL_AUTOMATON_CLOSURE_CONCEPTS_H
#define PASTEL_AUTOMATON_CLOSURE_CONCEPTS_H

#include "pastel/sys/automaton_closure.h"

#include "pastel/sys/reporter_concept.h"
#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/optional.h"

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

		class IsClosureSymbol
		{
		public:
			//! Returns whether the symbol takes part in closures.
			/*!
			The closure of a state A is the set of those states that
			can be reached from A through transitions with closure 
			symbols.
			*/
			bool operator()(
				const Optional<Symbol>& symbol) const;
		};

		class Closure_Reporter
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
