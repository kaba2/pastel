// Description: Single input
// Documentation: input.txt

#ifndef PASTELSYS_SINGLE_INPUT_H
#define PASTELSYS_SINGLE_INPUT_H

#include "pastel/sys/input_concept.h"

namespace Pastel
{

	template <typename Type>
	class Single_Input
	{
	public:
		Single_Input()
		: data_()
		, read_(false)
		{
		}

		explicit Single_Input(Type&& data)
		: data_(std::move(data))
		, read_(false)
		{
		}

		explicit Single_Input(const Type& data)
		: data_(data)
		, read_(false)
		{
		}

		integer nHint() const
		{
			return !read_;
		}

		bool empty() const
		{
			return read_;
		}

		const Type& get() const
		{
			return data_;
		}

		void pop()
		{
			read_ = true;
		}

	private:
		Type data_;
		bool read_;
	};

	template <typename Type>
	Single_Input<std::decay_t<Type>> singleInput(Type&& that)
	{
		return Single_Input<std::decay_t<Type>>(
			std::forward<Type>(that));
	}

}

#endif
