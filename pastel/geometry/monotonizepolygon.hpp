#ifndef PASTELGEOMETRY_MONOTONIZEPOLYGON_HPP
#define PASTELGEOMETRY_MONOTONIZEPOLYGON_HPP

#include "pastel/geometry/monotonizepolygon.h"

#include "pastel/sys/log.h"

namespace Pastel
{

	namespace Detail
	{

		namespace MonotonizePolygon
		{

			template <typename Real>
			class Vertex
			{
			public:
				class Type
				{
				public:
					enum Enum
					{
						Regular,
						Start,
						End,
						Split,
						Merge
					};
				};

				Vertex(
					const Point<2, Real>& position,
					integer index,
					typename Type::Enum type)
					: position_(position)
					, index_(index)
					, type_(type)
				{
				}

				bool operator<(const Vertex& that) const
				{
					if (position_.y() < that.position_.y())
					{
						return true;
					}
					if (that.position_.y() < position_.y())
					{
						return false;
					}
					if (position_.x() < that.position_.x())
					{
						return true;
					}
					if (that.position_.x() < position_.x())
					{
						return false;
					}

					return index_ < that.index_;
				}

				Point<2, Real> position_;
				integer index_;
				typename Type::Enum type_;
			};

			template <typename Real>
			class Edge
			{
			public:
				Edge(const Vertex<Real>* min,
					const Vertex<Real>* max)
					: min_(min)
					, max_(max)
					, helper_(0)
				{
				}

				const Vertex<Real>* min_;
				const Vertex<Real>* max_;
				const Vertex<Real>* helper_;
			};

			template <typename Real>
			class EdgeCompare
			{
			public:
				bool operator()(Edge<Real>* aEdge, Edge<Real>* bEdge) const
				{
					// In the following it is important
					// to keep in mind that the comparison
					// is only meaningful inside the sweep-line
					// algorithm. The comparison
					// is used when inserting and removing
					// elements from the status line to traverse
					// the status binary tree.

					// The sweep-line algorithm
					// guarantees that the
					// y-axis projections of the segments
					// overlap.
					// Therefore, one of the max points
					// is in the y-range of the other
					// segment.
					// This max-point is used as the
					// y-coordinate of the horizontal line
					// on which the x-coordinates are compared.

					if (aEdge->min_ != bEdge->min_)
					{
						if (aEdge->min_ <
							bEdge->min_)
						{
							// It is the 'bEdge' segment
							// that has the min point
							// on the shared y-range.

							const Real area = signedArea(
								aEdge->min_->position_,
								aEdge->max_->position_,
								bEdge->min_->position_);

							if (area > 0)
							{
								return false;
							}
							else if (area < 0)
							{
								return true;
							}
						}
						else
						{
							// It is the 'aEdge' segment
							// that has the max point
							// on the shared y-range.

							const Real area = signedArea(
								bEdge->min_->position_,
								bEdge->max_->position_,
								aEdge->min_->position_);

							if (area > 0)
							{
								return true;
							}
							else if (area < 0)
							{
								return false;
							}
						}
					}

					// Three points are collinear
					// (one of the min points may not be).

					const Real area = signedArea(
						bEdge->min_->position_,
						bEdge->max_->position_,
						aEdge->max_->position_);

					if (area < 0)
					{
						return false;
					}
					else if (area > 0)
					{
						return true;
					}

					// All points are collinear,
					// pick an arbitrary order.

					return aEdge < bEdge;
				}

			};

		}

	}

	template <typename Real, typename SplitFunctor>
	void monotonizePolygon(
		const std::vector<Point<2, Real> >& polygon,
		SplitFunctor& splitFunctor)
	{
		typedef Detail::MonotonizePolygon::Vertex<Real> Vertex;
		typedef Detail::MonotonizePolygon::Edge<Real> Edge;
		typedef std::set<Edge*, Detail::MonotonizePolygon::EdgeCompare<Real> > StatusSet;
		typedef StatusSet::iterator StatusIterator;

		const integer vertices = polygon.size();

		if (vertices < 4)
		{
			return;
		}

		StatusSet statusSet;

		std::vector<Vertex> vertexList;
		vertexList.reserve(vertices);

		for (integer i = 0;i < vertices;++i)
		{
			vertexList.push_back(
				Vertex(polygon[i], i, Vertex::Type::Regular));
		}

		std::vector<Edge> edgeList;
		edgeList.reserve(vertices);
		for (integer i = 0;i < vertices;++i)
		{
			integer iNext = i + 1;
			if (iNext == vertices)
			{
				iNext = 0;
			}
			if (vertexList[i] < vertexList[i + 1])
			{
				edgeList.push_back(Edge(&vertexList[i], &vertexList[iNext]));
			}
			else
			{
				edgeList.push_back(Edge(&vertexList[iNext], &vertexList[i]));
			}
		}

		// Classify vertices.

		for (integer i = 0;i < vertices;++i)
		{
			integer iPrevious = i - 1;
			if (iPrevious == -1)
			{
				iPrevious = vertices - 1;
			}
			integer iNext = i + 1;
			if (iNext == vertices)
			{
				iNext = 0;
			}

			Vertex& current = vertexList[i];
			const Vertex& previous = vertexList[iPrevious];
			const Vertex& next = vertexList[iNext];

			// Note that it is not possible
			// that
			// previous == current or
			// next == current or
			// previous == next

			if (current < previous &&
				current < next)
			{
				// Start-vertex or split-vertex.

				if (signedArea(current.position_, next.position_, previous.position_) > 0)
				{
					// A start-vertex
					current.type_ = Vertex::Type::Start;
				}
				else
				{
					// A split-vertex
					current.type_ = Vertex::Type::Split;
				}
			}
			else if (
				previous < current &&
				next < current)
			{
				// End-vertex or merge-vertex.

				if (signedArea(current.position_, next.position_, previous.position_) > 0)
				{
					// End-vertex
					current.type_ = Vertex::Type::End;
				}
				else
				{
					// Merge-vertex
					current.type_ = Vertex::Type::Merge;
				}
			}
			else
			{
				// Either previous < current && next > current
				// or next < current && previous > current.
				// Thus the vertex is a regular-vertex.

				current.type_ = Vertex::Type::Regular;
			}
			/*
			const char* names[] = {"regular", "start", "end", "split", "merge"};
			log() << names[(integer)current.type_] << " ";
			*/
		}

		// Sort the event points.

		std::vector<Vertex> sortedList(vertexList);

		std::sort(sortedList.begin(), sortedList.end());

		/*
		log() << logNewLine;

		for (integer i = 0;i < vertices;++i)
		{
			log() << vertexList[i].index_ << " ";
		}

		log() << logNewLine;
		*/

		for (integer i = 0;i < vertices;++i)
		{
			const integer iCurrent = sortedList[i].index_;

			integer iPrevious = iCurrent - 1;
			if (iPrevious == -1)
			{
				iPrevious = vertices - 1;
			}
			integer iNext = iCurrent + 1;
			if (iNext == vertices)
			{
				iNext = 0;
			}

			const Vertex& current = vertexList[iCurrent];
			const Vertex& previous = vertexList[iPrevious];
			const Vertex& next = vertexList[iNext];
			Edge& edge = edgeList[iCurrent];
			Edge& previousEdge = edgeList[iPrevious];
			Edge& nextEdge = edgeList[iNext];

			switch(current.type_)
			{
			case Vertex::Type::Start:
				{
					//log() << "Inserted edge " << iPrevious << logNewLine;
					statusSet.insert(&previousEdge);
					previousEdge.helper_ = &current;
					break;
				}
			case Vertex::Type::End:
				{
					const Vertex* helper = edge.helper_;
					if (helper &&
						helper->type_ == Vertex::Type::Merge)
					{
						splitFunctor(current.index_, helper->index_);
					}
					statusSet.erase(&edge);
					//log() << "Erased edge " << iCurrent << logNewLine;
					break;
				}
			case Vertex::Type::Split:
				{
					Edge searchEdge(&current, &next);
					ASSERT(current < next);
					StatusIterator iter = statusSet.upper_bound(&searchEdge);
					ASSERT(iter != statusSet.begin());
					--iter;

					Edge* leftEdge = *iter;
					ASSERT(leftEdge->helper_);
					splitFunctor(current.index_, leftEdge->helper_->index_);
					leftEdge->helper_ = &current;

					statusSet.insert(&previousEdge);
					//log() << "Inserted edge " << iPrevious << logNewLine;
					previousEdge.helper_ = &current;
					break;
				}
			case Vertex::Type::Merge:
				{
					const Vertex* helper = edge.helper_;
					if (helper &&
						helper->type_ == Vertex::Type::Merge)
					{
						splitFunctor(current.index_, helper->index_);
					}
					statusSet.erase(&edge);
					//log() << "Erased edge " << iCurrent << logNewLine;

					Edge searchEdge(&previous, &current);
					ASSERT(previous < current);
					StatusIterator iter = statusSet.upper_bound(&searchEdge);
					ASSERT(iter != statusSet.begin());
					--iter;

					Edge* leftEdge = *iter;
					const Vertex* leftHelper = leftEdge->helper_;
					if (leftHelper &&
						leftHelper->type_ == Vertex::Type::Merge)
					{
						splitFunctor(current.index_, leftHelper->index_);
					}
					leftEdge->helper_ = &current;
					break;
				}
			case Vertex::Type::Regular:
				{
					if (next < previous)
					{
						const Vertex* helper = edge.helper_;
						if (helper &&
							helper->type_ == Vertex::Type::Merge)
						{
							splitFunctor(current.index_, helper->index_);
						}
						statusSet.erase(&edge);
						//log() << "Erased edge " << iCurrent << logNewLine;

						statusSet.insert(&previousEdge);
						//log() << "Inserted edge " << iPrevious << logNewLine;
						previousEdge.helper_ = &current;
					}
					else
					{
						Edge searchEdge(&current, &next);
						ASSERT(current < next);
						StatusIterator iter = statusSet.upper_bound(&searchEdge);
						ASSERT(iter != statusSet.begin());
						--iter;

						Edge* leftEdge = *iter;
						const Vertex* leftHelper = leftEdge->helper_;
						if (leftHelper &&
							leftHelper->type_ == Vertex::Type::Merge)
						{
							splitFunctor(current.index_, leftHelper->index_);
						}
						leftEdge->helper_ = &current;
					}
					break;
				}
			};
		}
	}

}

#endif
