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
		using TransitionData_Class::operator=;

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

	private:
		TransitionLabel() PASTEL_DELETE;
		TransitionLabel(const TransitionLabel& that) PASTEL_DELETE;

		// Making the operator= private would give the error
		// C2876: "not all overloads are accessible".
		// FIX: Remove public when 'delete' becomes available.
	public:
		TransitionLabel& operator=(TransitionLabel that) PASTEL_DELETE;

	private:
		template <
			typename Symbol, 
			typename StateData, 
			typename TransitionData,
			typename Customization>
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
