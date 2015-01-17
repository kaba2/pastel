// Description: Point as a vector expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_POINT_VECTOREXPRESSION_H
#define PASTELSYS_POINT_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"

namespace Pastel
{

	template <
		typename Point,
		typename Locator>
	class Point_VectorExpression
		: public VectorExpression<
			typename Locator::Real, Locator::N, 
			Point_VectorExpression<Point, Locator>>
	{
	public:
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR integer N = Locator::N;

		using StorageType = const Point_VectorExpression;

		Point_VectorExpression()
			: point_()
			, locator_()
		{
		}

		Point_VectorExpression(
			const Point& point,
			const Locator& locator)
			: point_(point)
			, locator_(locator)
		{
		}

		integer size() const
		{
			return locator_.n();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		integer n() const
		{
			return locator_.n();
		}

		void swap(Point_VectorExpression& that)
		{
			using std::swap;
			
			locator_.swap(that.locator_);
			swap(point_, that.point_);
		}

		const Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size(), 
				index, size());

			return locator_(point_, index);
		}

	private:
		Point point_;
		Locator locator_;
	};

	template <typename Point, typename Locator>
	Point_VectorExpression<Point, Locator> pointAsVector(
		const Point& point,
		const Locator& locator)
	{
		return Point_VectorExpression<Point, Locator>(point, locator);
	}

}

#endif
