#ifndef PASTEL_AUTOMATON_CONCEPTS_H
#define PASTEL_AUTOMATON_CONCEPTS_H

#include "pastel/sys/automaton_fwd.h"

namespace Pastel
{

	namespace Automaton_Concepts
	{

		template <
			typename Symbol,
			typename StateData,
			typename TransitionData>
		class Customization
		{
		public:
			// The new member functions.

			void newMemberFunction() {}

		protected:
			typedef Automaton_Fwd<Symbol, StateData, TransitionData> Fwd;
		
			PASTEL_FWD(State_Iterator);
			PASTEL_FWD(State_ConstIterator);
			PASTEL_FWD(Transition_Iterator);
			PASTEL_FWD(Transition_ConstIterator);

			typedef Automaton<Symbol, StateData, TransitionData,
				Customization> Automaton;

			// The constructors and assignment should be protected to avoid
			// the possibility of splicing.

			Customization() {}
			Customization(const Customization&) {}
			Customization(Customization&&) {}
			Customization& operator=(
				Customization) {return *this;}

			//! Called at the start of swap().
			/*!
			Time complexity:
			O(1)

			Exception safety:
			nothrow
			*/
			void swap(Customization& that) {}

			// The customization functions.

			//! Called at the start of clear().
			/*!
			Exception safety:
			nothrow
			*/
			void onClear() {}

			//! Called at the start of clearTransitions().
			/*!
			Exception safety:
			nothrow
			*/
			void onClearTransitions() {}

			//! Called at the start of clearStart().
			/*!
			Exception safety:
			nothrow
			*/
			void onClearStart() {}

			//! Called at the start of clearFinal().
			/*!
			Exception safety:
			nothrow
			*/
			void onClearFinal() {}

			//! Called at the end of addState().
			/*!
			Exception safety:
			basic or stronger
			*/
			void onAddState(const State_Iterator& state) {}

			//! Called at the start of removeState().
			/*!
			Exception safety:
			nothrow
			*/
			void onRemoveState(const State_ConstIterator& state) {}

			//! Called at the end of addStart().
			/*!
			Exception safety:
			basic or stronger
			*/
			void onAddStart(
				const State_ConstIterator& state) {}

			//! Called at the start of removeStart().
			/*!
			Exception safety:
			nothrow
			*/
			void onRemoveStart(
				const State_ConstIterator& state) {}
			
			//! Called at the end of addFinal().
			/*!
			Exception safety:
			basic or stronger
			*/
			void onAddFinal(
				const State_ConstIterator& state) {}

			//! Called at the start of removeFinal().
			/*!
			Exception safety:
			nothrow
			*/
			void onRemoveFinal(
				const State_ConstIterator& state) {}
			
			//! Called at the end of addTransition().
			/*!
			Exception safety:
			basic or stronger
			*/
			void onAddTransition(
				const Transition_ConstIterator& transition)	{}

			//! Called at the start of removeTransition().
			/*!
			Exception safety:
			nothrow
			*/
			void onRemoveTransition(
				const Transition_ConstIterator& transition)	{}

			//! Called at the start of merge().
			/*!
			Exception safety:
			basic or stronger
			*/
			void onMerge(Automaton& that) {}
		};

	}

}

#endif
