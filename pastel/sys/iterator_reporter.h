// Description: Iterator reporter

#ifndef PASTEL_ITERATOR_REPORTER_H
#define PASTEL_ITERATOR_REPORTER_H

#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	template <typename Iterator>
	class Iterator_Reporter
	{
	public:
		explicit Iterator_Reporter(Iterator& iter)
			: iter_(iter)
		{
		}

		template <typename Type>
		void operator()(const Type& data) const
		{
			*iter = data;
			++iter;
		}

	private:
		Iterator& iter_;
	};

	template <typename Iterator>
	Iterator_Reporter<Iterator> iteratorReporter(
		Iterator& iter)
	{
		return Iterator_Reporter<Iterator>(iter);
	}

}

#endif
