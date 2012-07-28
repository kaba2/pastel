#ifndef PASTEL_AUTOMATON_TRANSITION_H
#define PASTEL_AUTOMATON_TRANSITION_H

#include "pastel/sys/automaton.h"

// Visual Studio generates "multiple assignment operators" warning,
// because it does not implement the deletion of functions 
// (which we simulate below).
#if (defined _WIN32 || defined _WIN64)
#	pragma warning(push)
#	pragma warning(disable: 4522)
#endif

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class Automaton_Fwd<Symbol, StateData, TransitionData>::Label
		: public TransitionData_Class
	{
	public:
		using TransitionData_Class::operator=;

		// FIX: Delete after emplace becomes available in Visual Studio.
		Label(Label&& that)
			: TransitionData_Class(std::move((TransitionData_Class&&)that))
			, symbol_(std::move(that.symbol_))
		{
		}

		const Symbol& symbol() const
		{
			return symbol_;
		}

	private:
		Label() PASTEL_DELETE;
		Label(const Label& that) PASTEL_DELETE;
		Label& operator=(Label that) PASTEL_DELETE;

		template <
			typename Symbol, 
			typename StateData, 
			typename TransitionData>
		friend class Automaton;

		Label(
			Symbol symbol,
			TransitionData_Class transitionData)
			: TransitionData_Class(std::move(transitionData))
			, symbol_(symbol)
		{
		}

		Symbol symbol_;
	};

}

#if (defined _WIN32 || defined _WIN64)
#	pragma warning(pop)
#endif

#endif
