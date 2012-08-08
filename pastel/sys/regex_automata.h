#ifndef PASTEL_REGEX_AUTOMATA_H
#define PASTEL_REGEX_AUTOMATA_H

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
		Automaton<Symbol, StateData, TransitionData, Customization> right)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;
	
	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularOptional(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization>
	auto regularKleeneStar(
		Automaton<Symbol, StateData, TransitionData, Customization> automaton)
		-> Automaton<Symbol, StateData, TransitionData, Customization>;

}

#include "pastel/sys/regex_automata.hpp"

#endif
