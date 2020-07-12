#include "test_pastelgfx.h"

#include <iostream>

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/texture.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/extender/arrayextender.h"
#include "pastel/sys/extender/indexextenders.h"

#include <memory>

using namespace Pastel;

void gfxInitialize()
{
	auto textureImage = std::make_shared<Array<Color>>();
	loadPcx("lena.pcx", *textureImage);

	auto grayImage = std::make_shared<Array<real32>>(textureImage->extent());
	std::transform(
		textureImage->begin(), textureImage->end(),
		grayImage->begin(), luma);
	
	auto mipMap = std::make_shared<MipMap<Color>>(constArrayView(*textureImage));
	auto texture = std::make_shared<EwaImage_Texture<Color>>(*mipMap, ArrayExtender<2, Color>(mirrorExtender()));
	//NearestImage_Texture<Color> texture(textureImage, ArrayExtender<2, Color>(mirrorExtender()));
	transform(*mipMap, fitColor);

	//grayImage.setExtent(128, 128);

	gfxStorage().set("lena_gray", grayImage);
	gfxStorage().set("lena_image", textureImage);
	gfxStorage().set("lena_mipmap", mipMap);
	gfxStorage().set("lena_texture", texture);
}

CallFunction run(gfxInitialize);

