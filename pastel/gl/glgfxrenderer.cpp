#include "pastel/gl/glgfxrenderer.h"
#include "pastel/geometry/planar_projection.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

#include "pastel/dsp/resample.h"
#include "pastel/dsp/gaussian_filter.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/indexextenders.h"

#include <GL/glew.h>

namespace Pastel
{

	namespace
	{

		void convert(const AffineTransformation2& from,
			AffineTransformation3& to)
		{
			Matrix3& toTransformation = to.matrix();
			const Matrix2& fromTransformation = from.matrix();

			toTransformation[0] = extend(fromTransformation[0], 0);
			toTransformation[1] = extend(fromTransformation[1], 0);
			toTransformation[2] = Vector3(0, 0, 1);
			to.translation() = extend(from.translation(), 0);
		}

		void convert(const Matrix4& matrix, float array[16])
		{
			array[0] = matrix(0, 0);
			array[1] = matrix(0, 1);
			array[2] = matrix(0, 2);
			array[3] = matrix(0, 3);

			array[4] = matrix(1, 0);
			array[5] = matrix(1, 1);
			array[6] = matrix(1, 2);
			array[7] = matrix(1, 3);

			array[8] = matrix(2, 0);
			array[9] = matrix(2, 1);
			array[10] = matrix(2, 2);
			array[11] = matrix(2, 3);

			array[12] = matrix(3, 0);
			array[13] = matrix(3, 1);
			array[14] = matrix(3, 2);
			array[15] = matrix(3, 3);
		}

		void convert(const AffineTransformation3& transformation,
			float array[16])
		{
			convert(asMatrix(transformation), array);
		}

	}

	GlGfxRenderer::GlGfxRenderer()
		: GfxRenderer<Color>()
	{
	}

	GlGfxRenderer::~GlGfxRenderer()
	{
	}

	void GlGfxRenderer::setGlViewport(
		const AlignedBox2i& viewport)
	{
		glViewport(viewport.min().x(), viewport.min().y(),
			viewport.extent()[0], viewport.extent()[1]);

		setViewWindow(viewWindow());
	}

	// Transformation state

	void GlGfxRenderer::onSetViewTransformation()
	{
		updateModelView();
	}

	void GlGfxRenderer::onSetModelTransformation()
	{
		updateModelView();
	}

	void GlGfxRenderer::onSetViewWindow()
	{
		Matrix4 projection;

		setOrthogonalProjection(
			viewWindow(),
			-1, 1,
			projection);

		float glProjection[16];
		convert(projection, glProjection);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glProjection);
	}

	void GlGfxRenderer::onSetResamplingMode()
	{
		glBindTexture(GL_TEXTURE_2D, texture());

		switch(resamplingMode())
		{
		case ResamplingMode::Nearest:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_NEAREST);
			break;
		case ResamplingMode::Trilinear:
			// Fall-through.
		case ResamplingMode::Anisotropic:
			// Fall-through.
		case ResamplingMode::Bilinear:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);
			break;
		};
	}

	void GlGfxRenderer::onSetTexture()
	{
		glBindTexture(GL_TEXTURE_2D, texture());
	}

	// Drawing

	integer GlGfxRenderer::uploadTexture(const Array<Color, 2>& image)
	{
		glEnable(GL_TEXTURE_2D);

		GLuint textureId;

		// Allocate a texture name
		glGenTextures(1, &textureId);

		// Select our current texture
		glBindTexture(GL_TEXTURE_2D, textureId);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Bilinear filtering for minification
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR);

		// Bilinear filtering for magnification
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// the texture wraps over at the edges (repeat)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		integer width = image.width();
		integer height = image.height();

		Array<uint8, 2> loadImage;

		if (!GLEW_ARB_texture_non_power_of_two &&
			(!isPowerOfTwo(width) ||
			!isPowerOfTwo(height)))
		{
			integer width2 = roundUpToPowerOf2(width);
			integer height2 = roundUpToPowerOf2(height);

			if (width2 >= 4096)
			{
				width2 = 2048;
			}
			if (height2 >= 4096)
			{
				height2 = 2048;
			}

			log() << "Resampling a texture for opengl consumption..."
				<< width << " x " << height << " -> "
				<< width2 << " x " << height2 << logNewLine;

			Array<Color, 2> resampledImage(width2, height2);
			resample<Color>(constArrayView(image), clampExtender(),
				gaussianFilter(2), arrayView(resampledImage));
			transform(arrayView(resampledImage), fitColor);

			loadImage.setExtent(width2 * 4, height2);

			for (integer y = 0;y < height2;++y)
			{
				for (integer x = 0;x < width2;++x)
				{
					const Color& color = resampledImage(x, y);
					loadImage(x * 4 + 0, y) = (uint8)255 * color[0];
					loadImage(x * 4 + 1, y) = (uint8)255 * color[1];
					loadImage(x * 4 + 2, y) = (uint8)255 * color[2];
					loadImage(x * 4 + 3, y) = (uint8)255;
				}
			}

			void* data = (void*)&loadImage(0, 0);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2,
				0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			loadImage.setExtent(width * 4, height);

			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					const Color& color = image(x, y);
					loadImage(x * 4 + 0, y) = (uint8)255 * color[0];
					loadImage(x * 4 + 1, y) = (uint8)255 * color[1];
					loadImage(x * 4 + 2, y) = (uint8)255 * color[2];
					loadImage(x * 4 + 3, y) = (uint8)255;
				}
			}

			void* data = (void*)&loadImage(0, 0);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		return textureId;
	}

	void GlGfxRenderer::clear() const
	{
		const Color& currentColor = color();

		glDisable(GL_DEPTH_TEST);
		glClearColor(currentColor[0], currentColor[1], currentColor[2], 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GlGfxRenderer::drawPoint(
		const Vector2& point) const
	{
		const Color& currentColor = color();

		glBegin(GL_POINTS);
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(point.x(), point.y());
		glEnd();
	}

	void GlGfxRenderer::drawSegment(
		const Segment2& segment) const
	{
		const Vector2& start = segment.start();
		const Vector2& end = segment.end();
		const Color& currentColor = color();

		glBegin(GL_LINES);
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(start.x(), start.y());
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(end.x(), end.y());
		glEnd();
	}

	void GlGfxRenderer::drawTriangle(
		const Triangle2& triangle) const
	{
		if (filled())
		{
			glBegin(GL_TRIANGLES);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		const Color& currentColor = color();

		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(triangle[0].x(), triangle[0].y());
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(triangle[1].x(), triangle[1].y());
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glVertex2f(triangle[2].x(), triangle[2].y());
		glEnd();
	}

	void GlGfxRenderer::drawTriangle(
		const Triangle2& triangle,
		const Tuple<Color, 3>& colorTriangle) const
	{
		if (filled())
		{
			glBegin(GL_TRIANGLES);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		glColor3f(colorTriangle[0][0], colorTriangle[0][1], colorTriangle[0][2]);
		glVertex2f(triangle[0].x(), triangle[0].y());
		glColor3f(colorTriangle[1][0], colorTriangle[1][1], colorTriangle[1][2]);
		glVertex2f(triangle[1].x(), triangle[1].y());
		glColor3f(colorTriangle[2][0], colorTriangle[2][1], colorTriangle[2][2]);
		glVertex2f(triangle[2].x(), triangle[2].y());
		glEnd();
	}

	void GlGfxRenderer::drawTriangle(
		const Triangle2& triangle,
		const Triangle2& textureTriangle) const
	{
		if (!filled())
		{
			drawTriangle(triangle);
			return;
		}

		const Color& currentColor = color();

		glEnable(GL_TEXTURE_2D);

		glBegin(GL_TRIANGLES);
		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glTexCoord2f(textureTriangle[0].x(), textureTriangle[0].y());
		glVertex2f(triangle[0].x(), triangle[0].y());

		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glTexCoord2f(textureTriangle[1].x(), textureTriangle[1].y());
		glVertex2f(triangle[1].x(), triangle[1].y());

		glColor3f(currentColor[0], currentColor[1], currentColor[2]);
		glTexCoord2f(textureTriangle[2].x(), textureTriangle[2].y());
		glVertex2f(triangle[2].x(), triangle[2].y());

		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	// Private

	void GlGfxRenderer::updateModelView()
	{
		AffineTransformation3 viewTransformation3;
		convert(viewTransformation(), viewTransformation3);

		AffineTransformation3 modelTransformation3;
		convert(modelTransformation(), modelTransformation3);

		const AffineTransformation3 invView3(
			inverse(viewTransformation3));

		const AffineTransformation3 modelView3(
			modelTransformation3 * invView3);

		float glModelView[16];
		convert(modelView3, glModelView);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glModelView);
	}

}
