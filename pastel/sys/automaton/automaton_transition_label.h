#ifndef PASTELSYS_AUTOMATON_TRANSITION_LABEL_H
#define PASTELSYS_AUTOMATON_TRANSITION_LABEL_H

#include "pastel/sys/automaton/automaton_fwd.h"
#include "pastel/sys/optional/optional.h"

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
			typename StateData_, 
			typename TransitionData_>
		class TransitionLabel
		{
		public:
			using Fwd = Automaton_Fwd<Symbol, StateData_, TransitionData_>;

			PASTEL_FWD(TransitionData);

			// FIX: Delete after emplace becomes available in Visual Studio.
			TransitionLabel(TransitionLabel&& that)
				: data_(std::move((TransitionData&&)that))
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
				data() = std::forward<Type>(that);
				return *this;
			}

			TransitionData& data() {
				return data_;
			}

			const TransitionData& data() const {
				return data_;
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
				TransitionData transitionData)
				: data_(std::move(transitionData))
				, symbol_(std::move(symbol))
			{
			}

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			TransitionData data_;
			Optional<Symbol> symbol_;
		};

	}

}

#endif
