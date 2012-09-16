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
		enum
		{
			FINAL_FLAG = 0x00000001,
			START_FLAG = 0x00000002
		};

	public:
		using StateData_Class::operator=;

		// FIX: Delete after emplace becomes available in Visual Studio.
		StateLabel(StateLabel&& that)
			: StateData_Class(std::move((StateData_Class&&)that))
			, finalPosition_(std::move(that.finalPosition_))
			, startPosition_(std::move(that.startPosition_))
			, flags_(std::move(that.flags_))
		{
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
		StateLabel() PASTEL_DELETE;
		StateLabel(const StateLabel& that) PASTEL_DELETE;
		
		// Making the operator= private would give the error
		// C2876: "not all overloads are accessible".
		// FIX: Remove public when 'delete' becomes available.
	public:
		StateLabel& operator=(StateLabel that) PASTEL_DELETE;

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
