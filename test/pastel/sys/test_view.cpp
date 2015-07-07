// Description: Testing for Views
// DocumentationOf: views.h

#include "test/test_init.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view.h"

namespace
{

	class RowVisitor
	{
	public:
		template <typename Contained_View>
		void operator()(
			const Contained_View& view) const
		{
		}
		template <typename Left_View, typename Right_View>
		void operator()(
			const Left_View& left,
			const Right_View& right) const
		{
		}
	};

}

TEST_CASE("RowVisit (View)")
{
	Array<int> a(Vector2i(1000, 1000));
	visitRows(constArrayView(a), 0, RowVisitor());
	visitRows(arrayView(a), 0, RowVisitor());
	visitRows(arrayView(a), arrayView(a), 0, RowVisitor());
	visitRows(constArrayView(a), arrayView(a), 0, RowVisitor());
	visitRows(constArrayView(a), constArrayView(a), 0, RowVisitor());
}

TEST_CASE("Trivial (View)")
{
	Array<int> a(Vector2i(1024, 1024));
	Array<int> b(a);
	a = b;
	b.clear();
	b = a;
	a.clear();
	a.setExtent(Vector2i(53, 45), 15);
}

