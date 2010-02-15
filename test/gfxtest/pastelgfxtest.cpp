#include "pastelgfxtest.h"

#include "pastel/sys/log_all.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/ewaimage_texture.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/dsp/mipmap_tools.h"

#include "pastel/sys/arrayextender.h"
#include "pastel/sys/indexextender_all.h"

using namespace Pastel;

#include <string>
#include <iostream>

using namespace std;

int main()
{
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	Array<Color> textureImage;
	loadPcx("lena.pcx", textureImage);
	
	MipMap<Color> mipMap(constArrayView(textureImage));
	EwaImage_Texture<Color> texture(mipMap, ArrayExtender<2, Color>(mirrorExtender()));
	//NearestImage_Texture<Color> texture(textureImage, ArrayExtender<2, Color>(mirrorExtender()));
	transform(mipMap, fitColor);

	gfxStorage().set("lena_image", &textureImage);
	gfxStorage().set("lena_mipmap", &mipMap);
	gfxStorage().set("lena_texture", &texture);

	gfxTestList().console();

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
