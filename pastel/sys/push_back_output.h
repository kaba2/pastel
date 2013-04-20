// Description: Push-back-into-a-container reporter

#ifndef PASTELSYS_PUSH_BACK_OUTPUT_H
#define PASTELSYS_PUSH_BACK_OUTPUT_H

#include "pastel/sys/output_concept.h"

namespace Pastel
{

	template <typename Container>
	class PushBack_Output
	{
	public:
		explicit PushBack_Output(Container& container)
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
	PushBack_Output<Container> pushBackOutput(
		Container& container)
	{
		return PushBack_Output<Container>(container);
	}

}

#endif
