// Description: IndexExtender class
// Documentation: indexextenders.txt

#ifndef PASTELSYS_INDEXEXTENDER_H
#define PASTELSYS_INDEXEXTENDER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/pointer/countedptr.h"

namespace Pastel
{

	class IndexExtender
		: public ReferenceCounted
	{
	public:
		virtual ~IndexExtender()
		{
		}

		virtual integer operator()(
			integer index, integer extent) const = 0;
	};

	using IndexExtenderPtr = CountedPtr<IndexExtender>;
	using ConstIndexExtenderPtr = CountedPtr<const IndexExtender>;

}

#endif
