#ifndef PASTEL_REGEX_H
#define PASTEL_REGEX_H

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSingleStart(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularEmpty()
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularEpsilon()
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSymbol(
		const PASTEL_NO_DEDUCTION(Symbol)& symbol)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularUnion(
		Automaton<Symbol, StateData, TransitionData, Customization> left,
		Automaton<Symbol, StateData, TransitionData, Customization> right)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSequence(
		Automaton<Symbol, StateData, TransitionData, Customization> left,
		Automaton<Symbol, StateData, TransitionData, Customization> right,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;
	
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularOptional(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularKleeneStar(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

}

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSingleStart(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		if (automaton.startStates() > 1)
		{
			State_ConstIterator start = 
				automaton.addState();

			std::for_each(
				automaton.cStartBegin(),
				automaton.cStartEnd(),
				[&](const State_ConstIterator& state)
			{
				automaton.removeStart(state);
				automaton.addTransition(
					start, epsilon, state);
			});

			automaton.addStart(rightStart);
		}

		return automaton;
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularEmpty()
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		return Automaton();
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularEpsilon()
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		Automaton automaton;
		automaton.addFinal(automaton.addState());

		return automaton;
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSymbol(
		const PASTEL_NO_DEDUCTION(Symbol)& symbol)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		Automaton automaton;

		State_ConstIterator start = 
			automaton.addState();
		automaton.addStart(start);

		State_ConstIterator final = 
			automaton.addState();
		automaton.addFinal(final);

		automaton.addTransition(
			start, symbol, final);

		return automaton;
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularUnion(
		Automaton<Symbol, StateData, TransitionData, Customization> left,
		Automaton<Symbol, StateData, TransitionData, Customization> right)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		left.merge(right);
		return left;
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSequence(
		Automaton<Symbol, StateData, TransitionData, Customization> left,
		Automaton<Symbol, StateData, TransitionData, Customization> right,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		right = regularSingleStart(std::move(right));

		if (right.startStates() == 0)
		{
			right.clearFinal();
		}
		else
		{
			State_ConstIterator rightStart =
				*right.cStartBegin();

			std::for_each(
				left.cFinalBegin(),
				left.cFinalEnd(),
				[&](const State_ConstIterator& state)
			{
				left.removeFinal(state);
				left.addTransition(
					state, epsilon, rightStart);
			});
		}

		left.merge(right);
			
		return left;
	}
	
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularOptional(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		if (automaton.startStates() > 1)
		{
			automaton = regularSingleStart(std::move(automaton));

			State_ConstIterator start = 
				*automaton.cStartBegin()
			automaton.addFinal(start);
		}
		else if (automaton.startStates() == 1)
		{
			State_ConstIterator start =
				automaton.cStateBegin();

			if (!start->final())
			{
				State_ConstIterator newStart = 
					automaton.addState();
				automaton.addStart(newStart);
				automaton.addFinal(newStart);

				automaton.removeStart(start);

				automaton.addTransition(
					newStart, epsilon, start);
			}
		}

		return automaton;
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularKleeneStar(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton,
		const PASTEL_NO_DEDUCTION(Symbol)& epsilon)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		automaton = regularOptional(automaton, epsilon);

		if (automaton.startStates() > 0)
		{
			State_ConstIterator start = 
				*automaton.cStartBegin();

			std::for_each(
				automaton.cFinalBegin(),
				automaton.cFinalEnd(),
				[&](const State_ConstIterator& state)
			{
				automaton.addTransition(
					state, epsilon, start); 
			});
		}

		return automaton;
	}

}

#endif
