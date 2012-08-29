// Description: Iterator reporter

#ifndef PASTELSYS_ITERATOR_REPORTER_H
#define PASTELSYS_ITERATOR_REPORTER_H

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

		template <typename That>
		bool operator()(That&& that) const
		{
			*iter = std::forward<That>(that);
			++iter;
			return true;
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
