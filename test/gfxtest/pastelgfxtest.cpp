#include "pastelgfxtest.h"

#include "pastel/sys/logging.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/ewaimage_texture.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/dsp/mipmap_tools.h"

#include "pastel/sys/arrayextender.h"
#include "pastel/sys/indexextenders.h"

using namespace Pastel;

#include <string>
#include <iostream>

using namespace std;

int main()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

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

	gfxTestList().console();

	generateTestReport(gfxTestReport(), log());

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
