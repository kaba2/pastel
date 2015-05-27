// Description: Point as a vector expression
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_POINT_VECTOREXPRESSION_H
#define PASTELSYS_POINT_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	template <typename Point>
	class Point_VectorExpression
		: public VectorExpression<
			Point_Real<Point>, 
			Locator_N<Point_Locator<Point>>::value, 
			Point_VectorExpression<Point>>
	{
	public:
		PASTEL_CONCEPT_CHECK(Point, Point_Concept);

		using Real = Point_Real<Point>;
		static constexpr integer N = 
			Locator_N<Point_Locator<Point>>::value;

		using StorageType = const Point_VectorExpression;

		Point_VectorExpression()
			: point_()
		{
		}

		Point_VectorExpression(const Point& point)
			: point_(point)
		{
		}

		integer size() const
		{
			return dimension(point_);
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
			return size();
		}

		void swap(Point_VectorExpression& that)
		{
			using std::swap;
			swap(point_, that.point_);
		}

		const Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size(), 
				index, size());

			return pointAxis(point_, index);
		}

	private:
		Point point_;
	};

	template <typename Point>
	Point_VectorExpression<Point> pointAsVector(
		const Point& point)
	{
		return Point_VectorExpression<Point>(point);
	}

}

#endif
