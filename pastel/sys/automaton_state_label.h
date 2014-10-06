#ifndef PASTELSYS_AUTOMATON_STATE_LABEL_H
#define PASTELSYS_AUTOMATON_STATE_LABEL_H

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
	class Automaton_Fwd<Symbol, StateData, TransitionData>::StateLabel
		: public StateData_Class
	{
	private:
		static PASTEL_CONSTEXPR uint32 FINAL_FLAG = 0x00000001;
		static PASTEL_CONSTEXPR uint32 START_FLAG = 0x00000002;
		
		typedef Automaton_Fwd<Symbol, StateData, TransitionData>
			Fwd;
		
		PASTEL_FWD(Final_Iterator);
		PASTEL_FWD(Start_Iterator);

	public:
		// FIX: Delete after emplace becomes available in Visual Studio.
		StateLabel(StateLabel&& that)
			: StateData_Class(std::move((StateData_Class&&)that))
			, finalPosition_(std::move(that.finalPosition_))
			, startPosition_(std::move(that.startPosition_))
			, flags_(std::move(that.flags_))
		{
		}

		//! Assigns to the contained data.
		template <typename Type>
		StateLabel& operator=(Type&& that)
		{
			((StateData_Class&)*this) = std::forward<Type>(that);
			return *this;
		}

		bool final() const
		{
			return (flags_ & FINAL_FLAG) != 0;
		}
		
		bool start() const
		{
			return (flags_ & START_FLAG) != 0;
		}

	private:
		StateLabel() = delete;
		StateLabel(const StateLabel& that) = delete;
		StateLabel& operator=(const StateLabel&) = delete;

	private:
		template <
			typename, typename, 
			typename, typename>
		friend class Automaton;

		StateLabel(
			StateData_Class stateData)
			: StateData_Class(std::move(stateData))
			, finalPosition_()
			, startPosition_()
			, flags_(0)
		{
		}

		void setStart(bool start)
		{
			flags_ |= START_FLAG;
			if (!start)
			{
				flags_ ^= START_FLAG;
			}
		}

		void setFinal(bool final)
		{
			flags_ |= FINAL_FLAG;
			if (!final)
			{
				flags_ ^= FINAL_FLAG;
			}
		}

		/*
		Is it really necessary to keep the flags here?
		The answer is yes for two reasons. First, we want a
		state to be able to tell whether its start/final 
		without consulting the automaton. Second, even if the 
		first reason wasn't included, we want startPosition
		and finalPosition to be container-independent; splicing 
		the startSet or the finalSet of an automaton to another 
		should not break anything (checking against startSet_.end() 
		or finalSet_.end() would break on splicing). 
		*/

		Final_Iterator finalPosition_;
		Start_Iterator startPosition_;
		uint32 flags_;
	};

}

#if (defined _WIN32 || defined _WIN64)
#	pragma warning(pop)
#endif

#endif
