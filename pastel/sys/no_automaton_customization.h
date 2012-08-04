#ifndef PASTEL_NO_AUTOMATON_CUSTOMIZATION_H
#define PASTEL_NO_AUTOMATON_CUSTOMIZATION_H

#include "pastel/sys/automaton_fwd.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class No_Automaton_Customization
	{
	protected:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData> Fwd;
		
		PASTEL_FWD(State_Iterator);
		PASTEL_FWD(State_ConstIterator);
		PASTEL_FWD(Transition_Iterator);
		PASTEL_FWD(Transition_ConstIterator);

		No_Automaton_Customization() {}
		No_Automaton_Customization(const No_Automaton_Customization&) {}
		No_Automaton_Customization(No_Automaton_Customization&&) {}
		No_Automaton_Customization& operator=(
			No_Automaton_Customization) {return *this;}

		void swap(No_Automaton_Customization& that)
		{
		}

		void onClear() {}
		void onClearTransitions() {}
		void onClearStart() {}
		void onClearFinal() {}

		void onAddState(const State_Iterator& state) {}
		void onRemoveState(const State_ConstIterator& state) {}

		void onAddStart(
			const State_ConstIterator& state) {}
		void onRemoveStart(
			const State_ConstIterator& state) {}
			
		void onAddFinal(
			const State_ConstIterator& state) {}
		void onRemoveFinal(
			const State_ConstIterator& state) {}
			
		void onAddTransition(
			const Transition_ConstIterator& transition)	{}
		void onRemoveTransition(
			const Transition_ConstIterator& transition)	{}

		void onMerge() {}
	};

}

#endif
