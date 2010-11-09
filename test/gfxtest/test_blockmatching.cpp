#include "pastelgfxtest.h"

#include <pastel/device/timer.h>

#include <pastel/math/euclidean_normbijection.h>

#include <pastel/dsp/discretecosine.h>

#include <pastel/math/qr_decomposition.h>

#include <pastel/geometry/pointset_eigen.h>
#include <pastel/geometry/pointkdtree_tools.h>
#include <pastel/geometry/search_all_neighbors_pointkdtree.h>

#include <pastel/gfx/matchblock_bruteforce.h>
#include <pastel/gfx/savepcx.h>

#include <pastel/sys/subarray_tools.h>
#include <pastel/sys/pointpolicy_all.h>
#include <pastel/sys/string_tools.h>

#include <fstream>

using namespace Pastel;

namespace
{

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
			testBruteForce();
			//testApproximate();
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
			
			bool dct = false;

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

			const integer blockSize = 64;
			
			const real32 maxRelativeError = 2;
			const integer kNearest = 64;
			const integer coeffs = 3;

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

			Array_PointPolicy<real32> pointPolicy(blockSize);

			if (!dct)
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

			typedef PointKdTree<real32, Dynamic, Array_PointPolicy<real32, Dynamic> >
				Tree;

			typedef Tree::ConstObjectIterator ConstObjectIterator;

			std::cout << "Building a kd-tree..." << std::endl;

			Tree tree(
				ofDimension(coeffs));

			std::vector<ConstObjectIterator> querySet;
			querySet.reserve(featureSet.height());

			tree.insert(
				sparseRange(
				countingIterator(featureSet.rawBegin()), 
				countingIterator(featureSet.rawEnd()), 
				coeffs),
				std::back_inserter(querySet));

			tree.refine(SlidingMidpoint2_SplitRule_PointKdTree());	
			//tree.refine(LongestMedian_SplitRule_PointKdTree());	
			//tree.refine(MinimumVolume_SplitRule_PointKdTree());	
			//tree.refine(Fair_SplitRule_PointKdTree());

			std::cout << "Tree depth " << depth(tree) << std::endl;
			//std::cout << "Tree objects " << tree.objects() << std::endl;

			//std::cout << tree.bound().extent() << " " << std::endl;

			std::cout << "Computing nearest neighbors..." << std::endl;

			Array<ConstObjectIterator> nearestSet(
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
								index = (nearestSet(k, myIndex)->object() - 
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
