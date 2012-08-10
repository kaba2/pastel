#ifndef PASTEL_REGEX_AUTOMATA_HPP
#define PASTEL_REGEX_AUTOMATA_HPP

#include "pastel/sys/regex_automata.h"

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

			while(automaton.startStates() > 0)
			{
				State_ConstIterator state =
					*automaton.cStartBegin();

				automaton.removeStart(state);
				automaton.addTransition(
					start, Epsilon(), state);
			}

			automaton.addStart(start);
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

		State_ConstIterator state = automaton.addState();

		automaton.addStart(state);
		automaton.addFinal(state);

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
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;

		if (left.useful())
		{
			if (right.useful())
			{
				left.merge(right);
			}
			return left;
		}
		else if (right.useful())
		{
			return right;
		}

		return Automaton();
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularSequence(
		Automaton<Symbol, StateData, TransitionData, Customization> left,
		Automaton<Symbol, StateData, TransitionData, Customization> right)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		if (!left.useful() || !right.useful())
		{
			// Either one accepts nothing. Thus the sequence
			// is the empty language.
			return Automaton();
		}

		right = regularSingleStart(std::move(right));

		State_ConstIterator rightStart =
			*right.cStartBegin();
		right.removeStart(rightStart);

		while(left.finalStates() > 0)
		{
			State_ConstIterator state =
				*left.cFinalBegin();

			left.removeFinal(state);
			left.addTransition(
				state, Epsilon(), rightStart);
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
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		if (!automaton.useful())
		{
			return regularEpsilon<Symbol, StateData, TransitionData, Customization>();
		}

		if (automaton.startStates() > 1)
		{
			automaton = regularSingleStart(std::move(automaton));

			State_ConstIterator start = 
				*automaton.cStartBegin();
			automaton.addFinal(start);
		}
		else
		{
			State_ConstIterator start =
				*automaton.cStartBegin();

			if (!start->final())
			{
				State_ConstIterator newStart = 
					automaton.addState();
				automaton.addStart(newStart);
				automaton.addFinal(newStart);

				automaton.removeStart(start);

				automaton.addTransition(
					newStart, Epsilon(), start);
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
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		if (!automaton.useful())
		{
			return regularEpsilon<Symbol, StateData, TransitionData, Customization>();
		}

		automaton = regularOptional(std::move(automaton));

		State_ConstIterator start = 
			*automaton.cStartBegin();

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			if (state != start)
			{
				automaton.addTransition(
					state, Epsilon(), start); 
			}
		});

		return automaton;
	}

}

#endif
