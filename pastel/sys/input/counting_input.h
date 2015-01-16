// Description: Counting input
// Documentation: input.txt

#ifndef PASTELSYS_COUNTING_INPUT_H
#define PASTELSYS_COUNTING_INPUT_H

#include "pastel/sys/input_concept.h"

namespace Pastel
{

	template <typename Incrementable>
	class Counting_Input
	{
	public:
		Counting_Input()
		: i_()
		, end_()
		{
		}

		explicit Counting_Input(
			const Incrementable& begin,
			const Incrementable& end)
		: i_(begin)
		, end_(end)
		{
		}

		integer nHint() const
		{
			return 0;
		}

		bool empty() const
		{
			return i_ == end_;
		}

		const Incrementable& get() const
		{
			PENSURE(!empty());
			return i_;
		}

		void pop()
		{
			PENSURE(!empty());
			++i_;
		}

	private:
		Incrementable i_;
		Incrementable end_;
	};

	template <typename Incrementable = integer>
	Counting_Input<Incrementable> countingInput(
		const Incrementable& begin,
		const Incrementable& end)
	{
		return Counting_Input<Incrementable>(begin, end);
	}

}

#endif
