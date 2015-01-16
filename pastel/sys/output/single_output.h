// Description: Single reporter

#ifndef PASTELSYS_SINGLE_OUTPUT_H
#define PASTELSYS_SINGLE_OUTPUT_H

#include "pastel/sys/output_concept.h"

namespace Pastel
{

	template <typename Type>
	class Single_Output
	{
	public:
		explicit Single_Output(Type& data)
			: data_(data)
		{
		}

		template <typename That>
		bool operator()(That&& that) const
		{
			data_ = std::forward<That>(that);
			return false;
		}

	private:
		Type& data_;
	};

	template <typename Type>
	Single_Output<Type> singleOutput(
		Type& data)
	{
		return Single_Output<Type>(data);
	}

}

#endif
