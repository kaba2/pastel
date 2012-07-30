#ifndef PASTEL_MINIMAL_AUTOMATON_H
#define PASTEL_MINIMAL_AUTOMATON_H

#include "pastel/sys/minimal_automaton_fwd.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class MinimalAutomaton
		: public MimimalAutomaton_Fwd<Symbol, StateData, TransitionData>
	{
	public:
		explicit MinimalAutomaton(
			Automaton automaton)
			: automaton_()
		{
			automaton_ = minimizeAutomaton(std::move(automaton));
		}

		const Automaton& automaton() const
		{
			return automaton_;
		}

		template <typename Symbol_ConstIterator>
		void insert(
			const Symbol_ConstIterator& begin,
			const Symbol_ConstIterator& end)
		{
			
		}

		template <typename Symbol_ConstIterator>
		void erase(
			const Symbol_ConstIterator& begin,
			const Symbol_ConstIterator& end)
		{
		}

		Automaton release()
		{
			Automaton result;
			automaton_.swap(result);
			return result;
		}

	private:
		Automaton automaton_;
	};

}

#endif
