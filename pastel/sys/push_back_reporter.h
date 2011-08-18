// Description: Push-back-into-a-container reporter

#ifndef PASTEL_PUSH_BACK_REPORTER_H
#define PASTEL_PUSH_BACK_REPORTER_H

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

		template <typename Type>
		void operator()(const Type& data) const
		{
			container_.push_back(data);
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
