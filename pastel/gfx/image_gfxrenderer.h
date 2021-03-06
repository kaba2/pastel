// Description: Image_GfxRenderer
// Detail: A 2d per-object software renderer to render to arrays.
// Documentation: renderer.txt

#ifndef PASTELGFX_IMAGE_GFXRENDERER_H
#define PASTELGFX_IMAGE_GFXRENDERER_H

#include "pastel/gfx/gfxrenderer.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/gfx/texture/nearestimage_texture.h"
#include "pastel/gfx/texture/linearimage_texture.h"
#include "pastel/gfx/texture/mipimage_texture.h"
#include "pastel/gfx/texture/ewaimage_texture.h"

#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename Type>
	class Image_GfxRenderer
		: public GfxRenderer<Type>
	{
	private:
		using Base = GfxRenderer<Type>;

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
			integer textureId, const MipMap<Type, 2>& mipMap);

		// Drawing

		virtual integer uploadTexture(const Array<Type, 2>& image);

		virtual void clear() const;

		virtual void drawPoint(
			const Vector2& point) const;

		virtual void drawSegment(
			const Segment2& segment) const;

		virtual void drawTriangle(
			const Triangle2& triangle) const;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<Type, 3>& colorTriangle) const;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture) const;

	private:
		Image_GfxRenderer(const Image_GfxRenderer&) = delete;
		Image_GfxRenderer& operator=(const Image_GfxRenderer&) = delete;

		void updateModelView();

		Vector2 transform(const Vector2& point) const;

		AffineTransformation<dreal, 2, 2> screenTransformation_;
		Array<Type>* image_;
		std::vector<const Array<Type>*> textureImage_;
		std::vector<const MipMap<Type>*> mipMap_;

		NearestImage_Texture<Type> nearestTexture_;
		LinearImage_Texture<Type> linearTexture_;
		MipImage_Texture<Type> mipTexture_;
		EwaImage_Texture<Type> ewaTexture_;

		Texture<Type>* texture_;
	};

}

#include "pastel/gfx/image_gfxrenderer.hpp"

#endif
