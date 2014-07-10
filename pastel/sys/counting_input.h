// Description: Counting input
// Documentation: inputs.txt

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
		{
		}

		explicit Counting_Input(
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

		Incrementable operator()()
		{
			return (i_++);
		}

	private:
		Incrementable i_;
	};

	template <typename Incrementable = integer>
	Counting_Input<Incrementable> countingInput(
		const Incrementable& begin = Incrementable())
	{
		return Counting_Input<Incrementable>(begin);
	}

}

#endif
