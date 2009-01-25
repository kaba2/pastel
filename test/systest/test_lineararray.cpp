#include "pastelsystest.h"

#include "pastel/sys/lineararray.h"

using namespace Pastel;

namespace
{

	namespace Detail
	{

		class NormalTag {};
		class TerminateTag {};

		template <int Index, int N, typename Cursor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Cursor& startCursor,
			NormalTag)
		{
			const integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitDimension<Index - 1>(
					extent, cursor, Tag());
				
				cursor.increment<Index>();
			}
		}

		template <int Index, int N, typename Cursor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Cursor& startCursor,
			TerminateTag)
		{
			const integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{
				*cursor = 0;								

				cursor.increment<Index>();
			}
		}

	}

	template <int N, typename Type>
	void visit(
		LinearArray<N, Type>& image)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail::NormalTag,
			Detail::TerminateTag>::type Tag;

		Detail::visitDimension<N - 1>(image.extent(), image.cursor(Point<N, integer>(0)), Tag());
	}

	void testTrivial()
	{
		LinearArray<3, int> b(100, 200, 300);

		for (integer x = 0;x < 100;++x)
		{
			for (integer y = 0;y < 200;++y)
			{
				for (integer z = 0;z < 300;++z)
				{
					b(x, y, z) = x * y * z;
				}
			}
		}

		visit(b);

		b.setExtent(100, 300, 200);
		b.setExtent(100, 200, 300);

		b.clear();

		REPORT1(b.width() != 0, b.width());
		REPORT1(b.height() != 0, b.height());
		REPORT1(b.depth() != 0, b.depth());

		b.setExtent(300, 200, 100);

		REPORT1(b.width() != 300, b.width());
		REPORT1(b.height() != 200, b.height());
		REPORT1(b.depth() != 100, b.depth());

		b.setExtent(0, 100, 200);
		
		REPORT1(b.width() != 0, b.width());
		REPORT1(b.height() != 0, b.height());
		REPORT1(b.depth() != 0, b.depth());
	}

	void testBegin()
	{
		testTrivial();
	}

	void addTest()
	{
		sysTestList().add("LinearArray", testBegin);
	}

	CallFunction run(addTest);

}
