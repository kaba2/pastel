// Description: emplace_back output

#ifndef PASTELSYS_EMPLACE_BACK_OUTPUT_H
#define PASTELSYS_EMPLACE_BACK_OUTPUT_H

#include "pastel/sys/output_concept.h"

namespace Pastel
{

	template <typename Container>
	class EmplaceBack_Output
	{
	public:
		explicit EmplaceBack_Output(Container& container)
			: container_(container)
		{
		}

		template <typename... That>
		bool operator()(That&&... that) const
		{
			container_.emplace_back(std::forward<That>(that)...);
			return true;
		}

	private:
		Container& container_;
	};

	template <typename Container>
	EmplaceBack_Output<Container> emplaceBackOutput(
		Container& container)
	{
		return EmplaceBack_Output<Container>(container);
	}

}

#endif
