// Description: Single reporter

#ifndef PASTEL_SINGLE_REPORTER_H
#define PASTEL_SINGLE_REPORTER_H

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

		template <typename T>
		void operator()(const T& data) const
		{
			data_ = data;
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
