#ifndef PASTEL_IMAGEGFXRENDERER_H
#define PASTEL_IMAGEGFXRENDERER_H

#include "pastel/gfx/gfxrenderer.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/gfx/nearestimagetexture.h"
#include "pastel/gfx/bilinearimagetexture.h"
#include "pastel/gfx/mipimagetexture.h"
#include "pastel/gfx/ewaimagetexture.h"

#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename Type>
	class ImageGfxRenderer
		: public GfxRenderer<Type>
	{
	private:
		typedef GfxRenderer<Type> Base;

	public:
		explicit ImageGfxRenderer(Array<2, Type>* image = 0);
		virtual ~ImageGfxRenderer();

		void setImage(Array<2, Type>* image);
		Array<2, Type>* image() const;

		void onSetViewTransformation();
		void onSetModelTransformation();
		void onSetTexture();
		void onSetResamplingMode();

		void uploadMipMap(
			integer textureId, const MipMap<2, Type>& mipMap);

		// Drawing

		virtual integer uploadTexture(const Array<2, Type>& image);

		virtual void clear();

		virtual void drawPoint(
			const Point2& point);

		virtual void drawSegment(
			const Segment2& segment);

		virtual void drawTriangle(
			const Triangle2& triangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<3, Type>& colorTriangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture);

	private:
		// Prohibited
		ImageGfxRenderer(const ImageGfxRenderer&);

		//Prohibited
		ImageGfxRenderer& operator=(const ImageGfxRenderer&);

		void updateModelView();

		Point2 transform(const Point2& point) const;

		AffineTransformation2 screenTransformation_;
		Array<2, Type>* image_;
		std::vector<const Array<2, Type>*> textureImage_;
		std::vector<const MipMap<2, Type>*> mipMap_;

		NearestImageTexture<Type> nearestTexture_;
		BilinearImageTexture<Type> bilinearTexture_;
		MipImageTexture<Type> mipTexture_;
		EwaImageTexture<Type> ewaTexture_;

		Texture<Type>* texture_;
	};

}

#include "pastel/gfx/imagegfxrenderer.hpp"

#endif
