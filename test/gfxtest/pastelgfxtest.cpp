#include "pastelgfxtest.h"

#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

using namespace Pastel;

#include <string>
#include <iostream>

#include "pastel/gfx/savepcx.h"

using namespace std;

int main()
{
	log().addObserver(LogObserverRef(new StreamLogObserver(&cout)));
	log().addObserver(LogObserverRef(new FileLogObserver("log.txt")));

	/*
	integer width = 100;
	integer height = 100;

	LinearArray<2, Color> image(width, height);
	for (integer y = 0;y < height;++y)
	{
		for (integer x = 0;x < width;++x)
		{
			image(x, y) = (((x + y) & 1) == 0) ? Color(1) : Color(0);
		}
	}

	savePcx(image, "grid.pcx");
	*/

	gfxTestList().console();

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
