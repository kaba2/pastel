// Description: Infinite counting input
// Documentation: inputs.txt

#ifndef PASTELSYS_INFINITE_COUNTING_INPUT_H
#define PASTELSYS_INFINITE_COUNTING_INPUT_H

#include "pastel/sys/input_concept.h"

namespace Pastel
{

	template <typename Incrementable>
	class Infinite_Counting_Input
	{
	public:
		Infinite_Counting_Input()
		: i_()
		{
		}

		explicit Infinite_Counting_Input(
			const Incrementable& begin)
		: i_(begin)
		{
		}

		integer nHint() const
		{
			return 0;
		}

		bool empty() const
		{
			return false;
		}

		const Incrementable& get() const
		{
			return i_;
		}

		void pop()
		{
			++i_;
		}

	private:
		Incrementable i_;
	};

	template <typename Incrementable = integer>
	Infinite_Counting_Input<Incrementable> countingInput(
		const Incrementable& begin = Incrementable())
	{
		return Infinite_Counting_Input<Incrementable>(begin);
	}

}

#endif
