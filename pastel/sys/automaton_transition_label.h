#ifndef PASTELSYS_AUTOMATON_TRANSITION_LABEL_H
#define PASTELSYS_AUTOMATON_TRANSITION_LABEL_H

#include "pastel/sys/automaton_fwd.h"
#include "pastel/sys/optional.h"

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	class Automaton_Fwd;

	namespace Automaton_
	{

		template <
			typename Symbol, 
			typename StateData, 
			typename TransitionData>
		class TransitionLabel
			: public Automaton_Fwd<Symbol, StateData, TransitionData>::TransitionData_Class
		{
		public:
			using Fwd = Automaton_Fwd<Symbol, StateData, TransitionData>;

			PASTEL_FWD(TransitionData_Class);

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
			friend class Pastel::Automaton;

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

}

#endif
