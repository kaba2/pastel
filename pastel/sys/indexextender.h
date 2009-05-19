#ifndef PASTEL_INDEXEXTENDER_H
#define PASTEL_INDEXEXTENDER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/countedptr.h"

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

	typedef CountedPtr<IndexExtender> IndexExtenderPtr;
	typedef CountedPtr<const IndexExtender> ConstIndexExtenderPtr;

}

#endif
