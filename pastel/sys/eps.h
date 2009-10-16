// Description: Next greater and next smaller floating point values
// Documentation: portability.txt

#ifndef PASTEL_EPS_H
#define PASTEL_EPS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/log.h"

namespace Pastel
{

	real64_ieee nextGreater(real64_ieee that);

	real64_ieee nextSmaller(real64_ieee that);

	real32_ieee nextGreater(real32_ieee that);

	real32_ieee nextSmaller(real32_ieee that);

}

namespace Pastel
{

	inline real64_ieee nextGreater(real64_ieee that)
	{
		if (that == nan<real64_ieee>() ||
			that == infinity<real64_ieee>() ||
			that == -infinity<real64_ieee>())
		{
			return nan<real64_ieee>();
		}

		uint64 bits = *((uint64*)&that);
	
		if (bits >> 63)
		{
			// Negative
			--bits;
		}
		else
		{
			++bits;
		}
		
		const real64 result = *((real64*)&bits);
		return result;
	}

	inline real64_ieee nextSmaller(real64_ieee that)
	{
		if (that == nan<real64_ieee>() ||
			that == infinity<real64_ieee>() ||
			that == -infinity<real64_ieee>())
		{
			return nan<real64_ieee>();
		}

		uint64 bits = *((uint64*)&that);
	
		if (bits >> 63)
		{
			// Negative
			++bits;
		}
		else
		{
			--bits;
		}
		
		const real64 result = *((real64*)&bits);
		return result;
	}

	inline real32_ieee nextGreater(real32_ieee that)
	{
		if (that == nan<real32_ieee>() ||
			that == infinity<real32_ieee>() ||
			that == -infinity<real32_ieee>())
		{
			return nan<real32_ieee>();
		}

		uint32 bits = *((uint32*)&that);

		if (bits >> 31)
		{
			// Negative
			--bits;
		}
		else
		{
			++bits;
		}
		
		const real32 result = *((real32*)&bits);
		return result;
	}

	inline real32_ieee nextSmaller(real32_ieee that)
	{
		if (that == nan<real32_ieee>() ||
			that == infinity<real32_ieee>() ||
			that == -infinity<real32_ieee>())
		{
			return nan<real32_ieee>();
		}

		uint32 bits = *((uint32*)&that);

		if (bits >> 31)
		{
			// Negative
			++bits;
		}
		else
		{
			--bits;
		}
		
		const real32 result = *((real32*)&bits);
		return result;
	}

}

#endif
