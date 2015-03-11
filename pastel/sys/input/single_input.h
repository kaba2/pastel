// Description: Scalar input
// Documentation: input.txt

#ifndef PASTELSYS_SCALAR_INPUT_H
#define PASTELSYS_SCALAR_INPUT_H

#include "pastel/sys/input/input_concept.h"

namespace Pastel
{

	template <typename Type>
	class Scalar_Input
	{
	public:
		Scalar_Input()
		: data_()
		, n_(1)
		{
		}

		explicit Scalar_Input(Type data, integer n = 1)
		: data_(std::move(data))
		, n_(n)
		{
			PENSURE_OP(n, >=, 0);
		}

		integer nHint() const
		{
			return n_;
		}

		integer n() const
		{
			return nHint();
		}

		bool empty() const
		{
			return n_ == 0;
		}

		const Type& get() const
		{
			PENSURE(!empty());
			return data_;
		}

		const Type& operator[](integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n());
			return data_;
		}

		void pop()
		{
			PENSURE(!empty());
			--n_;
		}

	private:
		Type data_;
		integer n_;
	};

	template <typename Type>
	Scalar_Input<typename std::decay<Type>::type> scalarInput(Type&& that, integer n = 1)
	{
		return Scalar_Input<typename std::decay<Type>::type>(
			std::forward<Type>(that), n);
	}

}

#endif
