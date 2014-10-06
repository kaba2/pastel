#ifndef PASTELSYS_AUTOMATON_TRANSITION_LABEL_H
#define PASTELSYS_AUTOMATON_TRANSITION_LABEL_H

#include "pastel/sys/automaton.h"
#include "pastel/sys/optional.h"

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
	class Automaton_Fwd<Symbol, StateData, TransitionData>::TransitionLabel
		: public TransitionData_Class
	{
	public:
		// FIX: Delete after emplace becomes available in Visual Studio.
		TransitionLabel(TransitionLabel&& that)
			: TransitionData_Class(std::move((TransitionData_Class&&)that))
			, symbol_(std::move(that.symbol_))
		{
		}

		const Optional<Symbol>& symbol() const
		{
			return symbol_;
		}

		//! Assigns to the contained data.
		template <typename Type>
		TransitionLabel& operator=(Type&& that)
		{
			((TransitionData_Class&)*this) = std::forward<Type>(that);
			return *this;
		}

	private:
		TransitionLabel() = delete;
		TransitionLabel(const TransitionLabel& that) = delete;
		TransitionLabel& operator=(const TransitionLabel& that) = delete;

	private:
		template <
			typename, typename, 
			typename, typename>
		friend class Automaton;

		TransitionLabel(
			Optional<Symbol> symbol,
			TransitionData_Class transitionData)
			: TransitionData_Class(std::move(transitionData))
			, symbol_(std::move(symbol))
		{
		}

		Optional<Symbol> symbol_;
	};

}

#if (defined _WIN32 || defined _WIN64)
#	pragma warning(pop)
#endif

#endif
