#include "pastelgfxtest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"

#include "pastel/math/matrix.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

namespace
{

	void testArray()
	{
		Array<Color, 2> image(100, 100);

		std::fill(image.rowBegin(20), image.rowEnd(20), Color(0, 1, 0));
		std::fill(image.columnBegin(30), image.columnEnd(30), Color(0, 0, 1));

		savePcx(image, "output/array_rowiterator.pcx");

		Matrix<Dynamic, Dynamic, real32> matrix(100, 100);
		std::fill(matrix.rowBegin(20), matrix.rowEnd(20), 1);
		std::fill(matrix.columnBegin(30), matrix.columnEnd(30), 0.5);

		saveGrayscalePcx(matrix.asArray(), "output/matrix_rowiterator.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Array.RowIterator", testArray);
	}

	CallFunction run(testAdd);

}
