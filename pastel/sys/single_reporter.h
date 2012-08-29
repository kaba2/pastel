// Description: Single reporter

#ifndef PASTELSYS_SINGLE_REPORTER_H
#define PASTELSYS_SINGLE_REPORTER_H

#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	template <typename Type>
	class Single_Reporter
	{
	public:
		explicit Single_Reporter(Type& data)
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
	Single_Reporter<Type> singleReporter(
		Type& data)
	{
		return Single_Reporter<Type>(data);
	}

}

#endif
