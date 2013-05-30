#include "test_pastelgfx.h"

#include "pastel/sys/logging.h"

#include <iostream>

using namespace Pastel;

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/ewaimage_texture.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/sys/arrayextender.h"
#include "pastel/sys/indexextenders.h"

using namespace Pastel;

int main(int argc, const char* argv[])
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	Array<Color> textureImage;
	loadPcx("lena.pcx", textureImage);

	Array<real32> grayImage(textureImage.extent());
	std::transform(
		textureImage.begin(), textureImage.end(),
		grayImage.begin(), luma);
	
	MipMap<Color> mipMap(constArrayView(textureImage));
	EwaImage_Texture<Color> texture(mipMap, ArrayExtender<2, Color>(mirrorExtender()));
	//NearestImage_Texture<Color> texture(textureImage, ArrayExtender<2, Color>(mirrorExtender()));
	transform(mipMap, fitColor);

	//grayImage.setExtent(128, 128);

	gfxStorage().set("lena_gray", &grayImage);
	gfxStorage().set("lena_image", &textureImage);
	gfxStorage().set("lena_mipmap", &mipMap);
	gfxStorage().set("lena_texture", &texture);

	if (argc > 1 && argv[1] == std::string("-r"))
	{
		setInvariantFailureAction(
			InvariantFailureAction::Throw);
		testRunner().run();
	}
	else
	{
		testRunner().console();
	}
	
	if (testReport().totalErrors() > 0)
	{
		generateTestReport(testReport(), log());
	}

	return testReport().totalErrors();
}



