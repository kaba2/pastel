// Description: Iterator reporter

#ifndef PASTELSYS_ITERATOR_OUTPUT_H
#define PASTELSYS_ITERATOR_OUTPUT_H

#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	template <typename Iterator>
	class Iterator_Output
	{
	public:
		explicit Iterator_Output(Iterator& iter)
			: iter_(iter)
		{
		}

		template <typename That>
		bool operator()(That&& that) const
		{
			*iter_ = std::forward<That>(that);
			++iter_;
			return true;
		}

	private:
		Iterator& iter_;
	};

	template <typename Iterator>
	Iterator_Output<Iterator> iteratorOutput(
		Iterator& iter)
	{
		return Iterator_Output<Iterator>(iter);
	}

}

#endif
