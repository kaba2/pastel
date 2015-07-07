#define CATCH_CONFIG_RUNNER
#include "test/test_init.h"

#include "test_pastelgfx.h"

#include "pastel/sys/logging.h"

#include <iostream>

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/texture.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/sys/extender/arrayextender.h"
#include "pastel/sys/extender/indexextenders.h"

int main(int argc, char* const argv[])
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

	return Catch::Session().run(argc, argv);
}

