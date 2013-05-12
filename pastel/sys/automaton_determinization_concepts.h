#ifndef PASTELSYS_AUTOMATON_DETERMINIZATION_CONCEPTS_H
#define PASTELSYS_AUTOMATON_DETERMINIZATION_CONCEPTS_H

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/hashed_tree.h"
#include "pastel/sys/optional.h"
#include "pastel/sys/iterator_address_predicate.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData>
	class Automaton_Determinization_Concepts
	{
	public:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData>
			Automaton;

		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		using StateSet = HashedTree<
			State_ConstIterator, 
			IteratorAddress_LessThan,
			IteratorAddress_Hash>;

		class State_Output
		{
		public:
			//! Reports a state of the deterministic automaton.
			/*!
			The state of the deterministic automaton is a set
			of states of the non-deterministic automaton. It
			is guaranteed that the memory address of the
			passed 'stateSet' given here will be preserved 
			over the reporting of the transitions. The second
			argument tells whether the state is the start 
			state of the deterministic automaton.
			*/
			void operator()(
				const StateSet& stateSet,
				bool startState) const;
		};

		class Transition_Output
		{
		public:
			//! Reports a transition of the deterministic automaton.
			void operator()(
				const StateSet& fromStateSet,
				const Optional<Symbol>& symbol,
				const StateSet& toStateSet) const;
		};
	};

}

#endif
