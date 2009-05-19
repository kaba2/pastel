#ifndef PASTEL_ABSTRACTARRAY_H
#define PASTEL_ABSTRACTARRAY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Type>
	class ConstAbstractArray
	{
	public:
		virtual ~ConstAbstractArray()
		{
		}

		virtual Vector<N, integer> extent() const = 0;

		virtual Type operator()(
			const Point<N, integer>& position) const = 0;
	};

	template <int N, typename Type>
	class AbstractArray
		: public ConstAbstractArray<N, Type>
	{
	public:
		virtual ~AbstractArray()
		{
		}

		virtual void setExtent(
			const Vector<N, integer>& extent) = 0;

		virtual void write(
			const Point<N, integer>& position,
			const Type& data) = 0;
	};

}

#endif
