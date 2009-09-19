#include <pastel/gfx/loadpcx.h>
#include <pastel/gfx/savepcx.h>
#include <pastel/gfx/color_space.h>
#include <pastel/gfx/color_illuminant.h>

#include "pastel/sys/view_all.h"

#include <iostream>
#include <string>

using namespace Pastel;
using namespace std;

Color hue(const Color& hsv)
{
	return hsvToRgb(Color(hsv[0], 1, 1));
}

Color saturation(const Color& hsv)
{
	return Color(hsv[1]);
}

Color value(const Color& hsv)
{
	return Color(hsv[2]);
}

int main()
{
	cout << "ColorChannels" << endl << endl;

	//cout << linearSrgbToXyzTransform() << endl;

	cout << xyzIlluminantD65() << endl;

	cout << "Input image (pcx): ";

	string inputFileName;
	cin >> inputFileName;

	Array<Color, 2> image;
	if (!loadPcx(inputFileName, image))
	{
		cout << "Error in image loading. Aborting." << endl;
		return 1;
	}

	transform(arrayView(image), rgbToHsv);

	Array<Color, 2> component;

	transform(constArrayView(image), arrayView(component), hue);
	savePcx(component, "hue.pcx");

	transform(constArrayView(image), arrayView(component), saturation);
	savePcx(component, "saturation.pcx");

	transform(constArrayView(image), arrayView(component), value);
	savePcx(component, "value.pcx");

	transform(arrayView(image), hsvToRgb);
	savePcx(image, "output.pcx");

	return 0;
}
