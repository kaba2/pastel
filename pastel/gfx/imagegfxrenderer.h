// Description: Image_GfxRenderer
// Detail: A 2d per-object software renderer to render to arrays.

#ifndef PASTEL_IMAGEGFXRENDERER_H
#define PASTEL_IMAGEGFXRENDERER_H

#include "pastel/gfx/gfxrenderer.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/gfx/nearestimage_texture.h"
#include "pastel/gfx/bilinearimage_texture.h"
#include "pastel/gfx/mipimage_texture.h"
#include "pastel/gfx/ewaimage_texture.h"

#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename Type>
	class Image_GfxRenderer
		: public GfxRenderer<Type>
	{
	private:
		typedef GfxRenderer<Type> Base;

	public:
		explicit Image_GfxRenderer(Array<Type, 2>* image = 0);
		virtual ~Image_GfxRenderer();

		void setImage(Array<Type, 2>* image);
		Array<Type, 2>* image() const;

		void onSetViewTransformation();
		void onSetModelTransformation();
		void onSetTexture();
		void onSetResamplingMode();

		void uploadMipMap(
			integer textureId, const MipMap<2, Type>& mipMap);

		// Drawing

		virtual integer uploadTexture(const Array<Type, 2>& image);

		virtual void clear();

		virtual void drawPoint(
			const Vector2& point);

		virtual void drawSegment(
			const Segment2& segment);

		virtual void drawTriangle(
			const Triangle2& triangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<Type, 3>& colorTriangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture);

	private:
		// Prohibited
		Image_GfxRenderer(const Image_GfxRenderer&);

		//Prohibited
		Image_GfxRenderer& operator=(const Image_GfxRenderer&);

		void updateModelView();

		Vector2 transform(const Vector2& point) const;

		AffineTransformation2 screenTransformation_;
		Array<Type, 2>* image_;
		std::vector<const Array<Type, 2>*> textureImage_;
		std::vector<const MipMap<2, Type>*> mipMap_;

		NearestImage_Texture<Type> nearestTexture_;
		BilinearImage_Texture<Type> bilinearTexture_;
		MipImage_Texture<Type> mipTexture_;
		EwaImage_Texture<Type> ewaTexture_;

		Texture<Type>* texture_;
	};

}

#include "pastel/gfx/imagegfxrenderer.hpp"

#endif
