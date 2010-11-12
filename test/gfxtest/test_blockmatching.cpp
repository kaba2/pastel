#include "pastelgfxtest.h"

#include <pastel/device/timer.h>

#include <pastel/math/euclidean_normbijection.h>

#include <pastel/dsp/discretecosine.h>

#include <pastel/math/qr_decomposition.h>

#include <pastel/geometry/pointset_eigen.h>
#include <pastel/geometry/pointkdtree_tools.h>
#include <pastel/geometry/search_all_neighbors_pointkdtree.h>
#include <pastel/geometry/windowedpointtree.h>

#include <pastel/gfx/matchblock_bruteforce.h>
#include <pastel/gfx/savepcx.h>

#include <pastel/sys/subarray_tools.h>
#include <pastel/sys/pointpolicy_all.h>
#include <pastel/sys/string_tools.h>
#include <pastel/sys/unorderedmap.h>
#include <pastel/sys/nulliterator.h>

#include <fstream>

using namespace Pastel;

/*
64D, 64N, full image
--------------------

Brute force, 1x memory, 3600s
L-median kd, 64x memory, 1269s
Sliding kd, 64x memory, 874s
Sliding kd + PCA, 64x memory, 360s
Sliding kd + PCA + 1eps, 64x memory, 179s
Sliding kd + PCA + 2eps, 64x memory, 118s

64D, 64N, 39x39 window
----------------------

Sliding kd + PCA, 64x memory, 510s
L-median kd, 8x memory, 328s

*/

namespace
{

	template <typename Real_, int N_, typename ConstIterator_>
	class Range_VectorExpression
		: public VectorExpression<
		Real_, N_, Range_VectorExpression<Real_, N_, ConstIterator_> >
	{
	public:
		enum {N = N_};

		typedef Real_ Real;

		typedef ConstIterator_ ConstIterator;

		Range_VectorExpression(
			const RandomAccessRange<ConstIterator>& range)
			: range_(range)
		{
		}				

		ConstIterator begin() const
		{
			return range_.begin();
		}

		ConstIterator end() const
		{
			return range_.end();
		}

		integer size() const
		{
			return range_.size();
		}

		const Real& operator[](integer index) const
		{
			return range_[index];
		}

		integer dimension() const
		{
			return range_.size();
		}

		bool involves(
			const void* memoryBegin, 
			const void* memoryEnd) const
		{
			return false;
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, 
			const void* memoryEnd) const
		{
			return false;
		}

	private:
		RandomAccessRange<ConstIterator> range_;
	};

	template <typename Real, int N, typename ConstIterator>
	Range_VectorExpression<Real, N, ConstIterator>
		rangeAsVector(
		const RandomAccessRange<ConstIterator>& range)
	{
		return Range_VectorExpression<Real, N, ConstIterator>(
			range);
	}

	class Separate_PointQuery
	{
	public:
		enum
		{
			M = 2
		};

		typedef Array_PointPolicy<real32> PointPolicy;

		Separate_PointQuery(
			const Array<real32>& image,
			const Vector<integer, 2>& blockExtent,
			Array<real32>& featureSet,
			bool dct = false, bool pca = false)
			: image_(image)
			, featureSet_(featureSet)
			, region_(VectorConstant<integer, M>(0, 2), image.extent() - blockExtent)
			, blockExtent_(blockExtent)
			, pointPolicy_(product(blockExtent))
		{
			const integer blockSize = product(blockExtent);
			const integer zigzag[8 * 8] = 
			{
				0,
				1, 8, 
				16, 9, 2,
				3, 10, 17, 24,
				32, 25, 18, 11, 4,
				5, 12, 19, 26, 33, 40,
				48, 41, 34, 27, 20, 13, 6,
				7, 14, 21, 28, 35, 42, 49, 56,
				57, 50, 43, 36, 29, 22, 15,
				23, 30, 37, 44, 51, 58,
				59, 52, 45, 38, 31,
				39, 46, 53, 60,
				61, 54, 47,
				55, 62,
				63
			};

			featureSet.setExtent(
				blockSize, (image.width() - 8) * (image.height() - 8));

#pragma omp parallel
			{
			Array<real32> block(8, 8);
#pragma omp for
			for (integer y = 0;y < image.height() - blockExtent.y();++y)
			{
				for (integer x = 0;x < image.width() - blockExtent.x();++x)
				{
					block() = image(Vector2i(x, y), Vector2i(x + blockExtent.x(), y + blockExtent.y()));
					for (integer i = 0;i < block.size();++i)
					{
						block(i) += random<real32>(0, inverse((real32)257));
					}

					const integer index = y * (image.width() - blockExtent.x()) + x;
					if (dct)
					{
						forEachRowOnAllAxes(block(), OrthogonalDct());
						for (integer i = 0;i < blockSize;++i)
						{
							featureSet(i, index) = block(zigzag[i]);
						}
					}
					else
					{
						for (integer i = 0;i < blockSize;++i)
						{
							featureSet(i, index) = block(i);
						}
					}
				}
			}
			}

			Array_PointPolicy<real32> pointPolicy(blockSize);

			if (pca && !dct)
			{
				std::cout << "Computing eigenstructure..." << std::endl;
				
				Matrix<real32> eigenVectorSet;
				Vector<real32> eigenValueSet;

				approximateEigenstructure(
					sparseRange(
					countingIterator(featureSet.rawBegin()), 
					countingIterator(featureSet.rawEnd()), 
					blockSize),
					pointPolicy,
					blockSize,
					eigenVectorSet,
					eigenValueSet);

				Vector<real32> mean(ofDimension(blockSize), 0);
				mean = pointSetMean(
					sparseRange(
					countingIterator(featureSet.rawBegin()), 
					countingIterator(featureSet.rawEnd()), 
					blockSize),
					pointPolicy);

				// We need to orthonormalize the matrix.
				QrDecomposition<real32> qr(transpose(eigenVectorSet));
				Vector<real32> t(ofDimension(blockSize));

				for (integer i = 0;i < featureSet.height();++i)
				{
					t = (pointPolicy(&featureSet(0, i)) - mean) * 
						transpose(qr.qTransposed());

					for (integer j = 0;j < featureSet.width();++j)
					{
						featureSet(j, i) = t[j];
					}
				}
			}
		}
	
		const PointPolicy& pointPolicy() const
		{
			return pointPolicy_;
		}

		template <typename Point_Functor>
		void queryPoints(
			const AlignedBox<integer, M>& region,
			Point_Functor report)
		{
			AlignedBox<integer, M> reportBox(
				ofDimension(region.dimension()));
			if (intersect(region, region_, reportBox) &&
				!reportBox.empty())
			{
				for (integer y = reportBox.min().y();y < reportBox.max().y();++y)
				{
					for (integer x = reportBox.min().x();x < reportBox.max().x();++x)
					{
						report(&featureSet_(0, y * (image_.width() - blockExtent_.x()) + x));
					}
				}
			}
		}

	private:
		const Array<real32>& image_;
		const Array<real32>& featureSet_;
		AlignedBox<integer, M> region_;
		Vector<integer, M> blockExtent_;
		PointPolicy pointPolicy_;
	};

	class Overlapping_PointQuery
	{
	public:
		enum
		{
			M = 2
		};

		typedef Array_PointPolicy<real32> PointPolicy;

		Overlapping_PointQuery(
			const Array<real32>& image,
			const Vector<integer, 2>& blockExtent,
			Array<real32>& featureSet)
			: image_(image)
			, featureSet_(featureSet)
			, region_(VectorConstant<integer, M>(0, 2), image.extent() - blockExtent)
			, blockExtent_(blockExtent)
			, pointPolicy_(product(blockExtent))
		{
			featureSet.setExtent(
				image.height() * blockExtent.x(), 
				image.width() - blockExtent.x());

			for (integer y = 0; y < image.height();++y)
			{
				for (integer x = 0;x < image.width() - blockExtent.x();++x)
				{
					std::copy(
						image.rowBegin(y) + x,
						image.rowBegin(y) + x + blockExtent.x(),
						featureSet.rowBegin(x) + blockExtent.x() * y);
				}
			}
		}

		const PointPolicy& pointPolicy() const
		{
			return pointPolicy_;
		}

		template <typename Point_Functor>
		void queryPoints(
			const AlignedBox<integer, M>& region,
			Point_Functor report)
		{
			AlignedBox<integer, M> reportBox(
				ofDimension(region.dimension()));
			if (intersect(region, region_, reportBox) &&
				!reportBox.empty())
			{
				for (integer y = reportBox.min().y();y < reportBox.max().y();++y)
				{
					for (integer x = reportBox.min().x();x < reportBox.max().x();++x)
					{
						report(&featureSet_(y * blockExtent_.x(), x));
					}
				}
			}
		}

	private:
		const Array<real32>& image_;
		const Array<real32>& featureSet_;
		AlignedBox<integer, M> region_;
		Vector<integer, M> blockExtent_;
		PointPolicy pointPolicy_;
	};

	class BlockMatching_Test
		: public TestSuite
	{
	public:
		BlockMatching_Test()
			: TestSuite(&gfxTestReport())
		{
		}

		virtual void run()
		{
			//testSimple();
			//testBruteForce();
			//testApproximate();
			testApproximate2();
		}

		void testSimple()
		{
			Array<real> image(512, 512, 0.5);
			Vector<integer, 2> blockExtent(8, 8);

			SubArray<real> region =
				image(Vector<integer, 2>(ofDimension(image.dimension()), 0), 
				image.extent() - blockExtent);

			SubArray<real>::Iterator iter = region.begin();
			for (integer i = 0;i < region.size();++i)
			{
				iter = region.begin(i);
				*iter = (real)i / (region.size() - 1);
				++iter;				
			}

			SubArray<real>::ConstIterator constIter = region.begin();
			ENSURE(std::equal(region.begin(), region.end(), constIter));

			saveGrayscalePcx(image, "subarray.pcx");
		}

		void testApproximate()
		{
			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");
		
			const integer blockSize = 64;
			const real32 maxRelativeError = 2;
			const integer kNearest = 64;
			const integer coeffs = 64;
			bool dct = false;
			bool pca = false;

			Timer timer;

			timer.setStart();

			integer zigzag[8 * 8] = 
			{
				0,
				1, 8, 
				16, 9, 2,
				3, 10, 17, 24,
				32, 25, 18, 11, 4,
				5, 12, 19, 26, 33, 40,
				48, 41, 34, 27, 20, 13, 6,
				7, 14, 21, 28, 35, 42, 49, 56,
				57, 50, 43, 36, 29, 22, 15,
				23, 30, 37, 44, 51, 58,
				59, 52, 45, 38, 31,
				39, 46, 53, 60,
				61, 54, 47,
				55, 62,
				63
			};

			Array<real32> featureSet(
				blockSize, (image.width() - 8) * (image.height() - 8));

			// Compute feature vectors

			std::cout << "Computing feature vectors..." << std::endl;

#pragma omp parallel
			{
			Array<real32> dctImage(8, 8);
#pragma omp for
			for (integer y = 0;y < image.height() - 8;++y)
			{
				for (integer x = 0;x < image.width() - 8;++x)
				{
					dctImage() = image(Vector2i(x, y), Vector2i(x + 8, y + 8));
					for (integer i = 0;i < dctImage.size();++i)
					{
						dctImage(i) += random<real32>(0, inverse((real32)257));
					}

					const integer index = y * (image.width() - 8) + x;
					if (dct)
					{
						forEachRowOnAllAxes(dctImage(), OrthogonalDct());
						for (integer i = 0;i < blockSize;++i)
						{
							featureSet(i, index) = dctImage(zigzag[i]);
						}
					}
					else
					{
						for (integer i = 0;i < blockSize;++i)
						{
							featureSet(i, index) = dctImage(i);
						}
					}
				}
			}
			}

			/*
			{
			std::ofstream file("feature.txt");
			for (integer i = 0;i < featureSet.height();++i)
			{
				for (integer j = 0;j < featureSet.width();++j)
				{
					file << featureSet(j, i) << " ";
				}
				file << std::endl;
			}
			}
			*/

			Array_PointPolicy<real32> pointPolicy(blockSize);

			if (pca && !dct)
			{
				std::cout << "Computing eigenstructure..." << std::endl;
				
				Matrix<real32> eigenVectorSet;
				Vector<real32> eigenValueSet;

				approximateEigenstructure(
					sparseRange(
					countingIterator(featureSet.rawBegin()), 
					countingIterator(featureSet.rawEnd()), 
					blockSize),
					pointPolicy,
					blockSize,
					eigenVectorSet,
					eigenValueSet);

				Vector<real32> mean(ofDimension(blockSize), 0);
				mean = pointSetMean(
					sparseRange(
					countingIterator(featureSet.rawBegin()), 
					countingIterator(featureSet.rawEnd()), 
					blockSize),
					pointPolicy);

				// We need to orthonormalize the matrix.
				QrDecomposition<real32> qr(transpose(eigenVectorSet));
				Vector<real32> t(ofDimension(blockSize));

				for (integer i = 0;i < featureSet.height();++i)
				{
					t = (pointPolicy(&featureSet(0, i)) - mean) * 
						transpose(qr.qTransposed());

					for (integer j = 0;j < featureSet.width();++j)
					{
						featureSet(j, i) = t[j];
					}
				}
			}

			// Dimensionality reduction
			featureSet.setExtent(
				coeffs, featureSet.height());
			
			pointPolicy = Array_PointPolicy<real32>(coeffs);

			Vector<real32> mean = pointSetMean(
				sparseRange(
				countingIterator(featureSet.rawBegin()), 
				countingIterator(featureSet.rawEnd()), 
				coeffs),
				pointPolicy);

			Vector<real32> stdev(ofDimension(coeffs), 0);

			stdev = sqrt(pointSetVariance(
				sparseRange(
				countingIterator(featureSet.rawBegin()), 
				countingIterator(featureSet.rawEnd()), 
				coeffs),
				mean.rawBegin(),
				pointPolicy));

			//std::cout << "Mean " << mean << std::endl;
			std::cout << "Stdev " << stdev << std::endl;

			typedef PointKdTree<real32, Dynamic, Array_PointPolicy<real32, Dynamic> >
				Tree;

			typedef Tree::ConstPointIterator ConstPointIterator;

			std::cout << "Building a kd-tree..." << std::endl;

			Tree tree(pointPolicy);

			std::vector<ConstPointIterator> querySet;
			querySet.reserve(featureSet.height());

			tree.insert(
				sparseRange(
				countingIterator(featureSet.rawBegin()), 
				countingIterator(featureSet.rawEnd()), 
				coeffs),
				std::back_inserter(querySet));

			//tree.refine(SlidingMidpoint_SplitRule_PointKdTree());	
			tree.refine(SlidingMidpoint2_SplitRule_PointKdTree());	
			//tree.refine(LongestMedian_SplitRule_PointKdTree());	
			//tree.refine(MinimumVolume_SplitRule_PointKdTree());	
			//tree.refine(Fair_SplitRule_PointKdTree());

			std::cout << "Tree depth " << depth(tree) << std::endl;
			//std::cout << "Tree points " << tree.points() << std::endl;

			//std::cout << tree.bound().extent() << " " << std::endl;

			std::cout << "Computing nearest neighbors..." << std::endl;

			Array<ConstPointIterator> nearestSet(
				kNearest, image.size(), tree.end());

			searchAllNeighbors(
				tree,
				randomAccessRange(querySet.begin(), querySet.end()),
				0, kNearest,
				&nearestSet,
				0,
				constantRange(infinity<real32>(), querySet.size()),
				maxRelativeError);

			timer.store();

			std::cout << "Computation took " << 
				timer.seconds() << " seconds." << std::endl;

			std::cout << "Saving results to file..." << std::endl;

			std::ofstream file("lena-64-nearest-8x8-kd.txt");
			for (integer i = 0;i < image.height();++i)
			{
				for (integer j = 0;j < image.width();++j)
				{
					if (i < image.height() - 8 && 
						j < image.width() - 8)
					{
						const integer myIndex = 
							i * (image.width() - 8) + j;

						const integer myReportedIndex =
							i * image.width() + j;

						file << myReportedIndex << " ";

						for (integer k = 0;k < kNearest - 1;++k)
						{
							integer index = -1;

							if (nearestSet(k, myIndex) != tree.end())
							{
								index = (nearestSet(k, myIndex)->point() - 
									featureSet.rawBegin()) / coeffs;

								const integer y = index / (image.width() - 8);
								const integer x = index - y * (image.width() - 8);

								index = y * image.width() + x;
							}

							file << index << " ";
						}
					}
					else
					{
						for (integer k = 0;k < kNearest;++k)
						{
							file << -1 << " ";
						}
					}
				}
				file << std::endl;
			}

			std::cout << "Done." << std::endl;
		}

		class Block_PointPolicy
		{
		public:
			Block_PointPolicy(
				const Array<real32>& image,
				integer blockWidth)
				: image_(&image)
				, blockWidth_(blockWidth)
			{
			}

			enum {N = Dynamic};
			
			typedef real32 Real;
			
			typedef const real32* Point;

			typedef ConstSubArray<real32>::ConstIterator ConstIterator;

			typedef Range_VectorExpression<real32, N, ConstIterator> 
				Expression;

			Expression operator()(const Point& point) const
			{
				return rangeAsVector<Real, N>(
					randomAccessRange(begin(point), end(point)));
			}

			ConstIterator begin(const Point& point) const
			{
				const Vector2i position = 
					image_->position(point - image_->rawBegin());

				return (*image_)(position, position + blockWidth_).begin();
			}

			//! Returns an iterator to the end of coordinate data.
			ConstIterator end(const Point& point) const
			{
				const Vector2i position = 
					image_->position(point - image_->rawBegin());

				return (*image_)(position, position + blockWidth_).end();
			}

			integer dimension() const
			{
				return square(blockWidth_);
			}

			integer dimension(const Point& point) const
			{
				return dimension();
			}				

		private:
			const Array<real32>* image_;
			integer blockWidth_;
		};

		void testApproximate2()
		{
			const integer blockWidth = 8;
			const real32 maxRelativeError = 2;
			const integer kNearest = 64;

			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");

			/*
			Array<real32> image(lenaImage);
			for (integer i = 0;i < image.size();++i)
			{
				image(i) += random<real32>(0, inverse((real32)257));
			}

			saveGrayscalePcx(image, "noisy_lena.pcx");
			*/

			Timer timer;
			timer.setStart();

			typedef Overlapping_PointQuery PointQuery;
			//typedef Separate_PointQuery PointQuery;
			typedef PointQuery::PointPolicy PointPolicy;
			typedef WindowedPointTree<PointQuery> WindowedTree;

			Array<real32> featureSet;

			PointQuery pointQuery(
				image, Vector2i(blockWidth), featureSet);

			const PointPolicy& pointPolicy = pointQuery.pointPolicy();

			//saveGrayscalePcx(featureSet, "feature_image.pcx");

			/*
			Array<real32> test(blockWidth, blockWidth);
			std::copy(
				pointPolicy(&featureSet(0, 1)).begin(),
				pointPolicy(&featureSet(0, 1)).end(),
				test.begin());

			saveGrayscalePcx(test, "test_image.pcx");
			*/

			/*
			{
			std::ofstream file("feature.txt");
			for (integer i = 0;i < image.height() - blockWidth;++i)
			{
				for (integer j = 0;j < image.width() - blockWidth;++j)
				{
					file << pointPolicy(&image(j, i)) << std::endl;
				}
			}
			}
			*/

			std::cout << "Building a kd-tree..." << std::endl;

			WindowedTree windowedTree(pointQuery);

			std::cout << "Tree depth " << depth(windowedTree.tree()) << std::endl;

			windowedTree.setWindow(
				AlignedBox2i(0, 0, 
				image.width() - blockWidth, image.height() - blockWidth));
			
			//windowedTree.refine(
			//	SlidingMidpoint2_SplitRule_PointKdTree());

			windowedTree.refine(
				LongestMedian_SplitRule_PointKdTree());

			std::cout << "Tree depth " << depth(windowedTree.tree()) << std::endl;
			std::cout << "Tree points " << windowedTree.tree().points() << std::endl;

			//std::cout << windowedTree.tree().bound().extent() << " " << std::endl;

			std::cout << "Computing nearest neighbors..." << std::endl;

			typedef WindowedTree::Tree Tree;
			typedef Tree::ConstPointIterator ConstPointIterator;

			const Tree& tree = windowedTree.tree();

			Array<ConstPointIterator> nearestSet(
				kNearest, image.size(), tree.end());

			Vector<real32> searchPoint(
				ofDimension(square(blockWidth)));

			AlignedBox<integer, 2> neighborhood(
				-19, -19, 20, 20);

			//AlignedBox<integer, 2> neighborhood(
			//	-10000, -10000, 10000, 10000);

			//log() << tree.points() << logNewLine;
			//return;

			bool xShiftRight = true;
			integer x = 0;
			for (integer y = 0;y < image.height() - blockWidth;++y)
			{
				log() << "y = " << y << logNewLine;

				while(true)
				{
					//log() << "x = " << x << logNewLine;

					windowedTree.setWindow(neighborhood);

					//log() << tree.points() << logNewLine;

					searchPoint = pointPolicy(&image(x, y));

					searchNearest(
						tree,
						searchPoint,
						kNearest,
						nearestSet.rowBegin(y * image.width() + x),
						NullIterator(),
						infinity<real32>(),
						maxRelativeError);

					if (xShiftRight)
					{
						if (x < image.width() - blockWidth - 1)
						{
							// Shift right in x-direction.
							++neighborhood.min().x();
							++neighborhood.max().x();
							++x;
						}
						else
						{
							break;
						}
					}
					else
					{
						if (x > 0)
						{
							// Shift left in x-direction.
							--neighborhood.min().x();
							--neighborhood.max().x();
							--x;
						}
						else
						{
							break;
						}
					}
				}

				// Change the direction of the shifting in
				// the x direction.
				xShiftRight = !xShiftRight;

				if (y < image.height() - blockWidth - 1)
				{
					// Shift in y-direction.
					++neighborhood.min().y();
					++neighborhood.max().y();
				}
			}

			timer.store();

			std::cout << "Computation took " << 
				timer.seconds() << " seconds." << std::endl;

			return;

			std::cout << "Saving results to file..." << std::endl;

			std::string filename = 
				std::string("lena-") + integerToString(kNearest) + 
				"-nearest-" + integerToString(blockWidth) + "x" + 
				integerToString(blockWidth) + "-kd.txt";

			std::ofstream file(filename.c_str());
			for (integer i = 0;i < image.height();++i)
			{
				for (integer j = 0;j < image.width();++j)
				{
					if (i < image.height() - 8 && 
						j < image.width() - 8)
					{
						const integer myIndex = 
							i * (image.width() - 8) + j;

						const integer myReportedIndex =
							i * image.width() + j;

						file << myReportedIndex << " ";

						for (integer k = 0;k < kNearest - 1;++k)
						{
							integer index = -1;

							if (nearestSet(k, myIndex) != windowedTree.tree().end())
							{
								index = nearestSet(k, myIndex)->point() - 
									image.rawBegin();

								const integer y = index / (image.width() - 8);
								const integer x = index - y * (image.width() - 8);

								index = y * image.width() + x;
							}

							file << index << " ";
						}
					}
					else
					{
						for (integer k = 0;k < kNearest;++k)
						{
							file << -1 << " ";
						}
					}
				}
				file << std::endl;
			}

			std::cout << "Done." << std::endl;
		}

		void testBruteForce()
		{
			const integer kNearest = 8;
			const integer blockSize = 8;

			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");
			
			Timer timer;

			timer.setStart();

			AlignedBox<integer, 2> neighborhood(
				-19, -19, 20, 20);

			Array<integer> nearestSet = matchBlockBrute(
				image, Vector2i(blockSize, blockSize), neighborhood, kNearest, 
				infinity<real32>(), Euclidean_NormBijection<real32>());

			timer.store();

			std::cout << "Computation took " << 
				timer.seconds() << " seconds." << std::endl;

			std::string filename = 
				std::string("lena-") + integerToString(kNearest) + 
				"-nearest-" + integerToString(blockSize) + "x" + 
				integerToString(blockSize) + "-brute.txt";

			std::ofstream file(filename.c_str());
			for (integer i = 0;i < nearestSet.height();++i)
			{
				for (integer j = 0;j < nearestSet.width();++j)
				{
					file << nearestSet(j, i) << " ";
				}
				file << std::endl;
			}

			//saveGrayscalePcx(image, "lena-64-nearest.pcx");
		}

	};

	void testBlockMatching()
	{
		BlockMatching_Test test;
		test.run();
	}

	void addTest()
	{
		gfxTestList().add("BlockMatching", testBlockMatching);
	}

	CallFunction run(addTest);

}
