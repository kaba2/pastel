// Description: Push-back-into-a-container reporter

#ifndef PASTELSYS_PUSH_BACK_REPORTER_H
#define PASTELSYS_PUSH_BACK_REPORTER_H

#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	template <typename Container>
	class PushBack_Reporter
	{
	public:
		explicit PushBack_Reporter(Container& container)
			: container_(container)
		{
		}

		template <typename That>
		bool operator()(That&& that) const
		{
			container_.push_back(std::forward<That>(that));
			return true;
		}

	private:
		Container& container_;
	};

	template <typename Container>
	PushBack_Reporter<Container> pushBackReporter(
		Container& container)
	{
		return PushBack_Reporter<Container>(container);
	}

}

#endif
