#include "pastelsystest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view_all.h"

using namespace Pastel;

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

	void testRowVisit()
	{
		Array<2, int> a(1000, 1000);
		visitRows(constArrayView(a), 0, RowVisitor());
		visitRows(arrayView(a), 0, RowVisitor());
		visitRows(arrayView(a), arrayView(a), 0, RowVisitor());
		visitRows(constArrayView(a), arrayView(a), 0, RowVisitor());
		visitRows(constArrayView(a), constArrayView(a), 0, RowVisitor());
	}

	void testTrivial()
	{
		Array<2, int> a(1024, 1024);
		Array<2, int> b(a);
		a = b;
		b.clear();
		b = a;
		a.clear();
		a.setExtent(53, 45, 15);
	}

	void testBegin()
	{
		testTrivial();
	}

	void addTest()
	{
		sysTestList().add("Array", testBegin);
	}

	CallFunction run(addTest);

}
