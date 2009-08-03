#include "pastelgfxtest.h"

#include "pastel/sys/log_all.h"

using namespace Pastel;

#include <string>
#include <iostream>

#include "pastel/gfx/savepcx.h"

using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

int main()
{
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	create_directory("output");

	/*
	integer width = 100;
	integer height = 100;

	Array<Color, 2> image(width, height);
	for (integer y = 0;y < height;++y)
	{
		for (integer x = 0;x < width;++x)
		{
			image(x, y) = even(x + y) ? Color(1) : Color(0);
		}
	}

	savePcx(image, "output/grid.pcx");
	*/

	gfxTestList().console();

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
