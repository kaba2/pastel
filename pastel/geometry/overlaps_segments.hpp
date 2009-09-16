#ifndef PASTEL_OVERLAPS_SEGMENTS_HPP
#define PASTEL_OVERLAPS_SEGMENTS_HPP

#include "pastel/geometry/overlaps_segments.h"
#include "pastel/geometry/overlaps_segment_segment.h"
#include "pastel/geometry/predicates.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/listarray.h"
#include "pastel/sys/log.h"

#include <map>
#include <vector>

#include <boost/operators.hpp>

namespace Pastel
{

	namespace Detail
	{

		template <typename Real>
		class OverlapSegments
		{
		private:
			class EventPoint
				: boost::less_than_comparable<
				EventPoint>
			{
			public:
				EventPoint()
					: id_(0)
					, position_()
				{
				}

				EventPoint(
					integer id,
					const Vector<Real, 2>& position)
					: id_(id)
					, position_(position)
				{
				}

				bool operator<(const EventPoint& right) const
				{
					const EventPoint& left = *this;

					return left.position_.x() < right.position_.x() ||
						(left.position_.x() == right.position_.x() &&
						left.position_.y() < right.position_.y()) ||
						(left.position_ == right.position_ &&
						left.id_ < right.id_);
				}

				integer id_;
				Vector<Real, 2> position_;
			};

			class StatusSegment
				: boost::less_than_comparable<
				StatusSegment>
			{
			public:
				StatusSegment()
					: start_()
					, end_()
				{
				}

				StatusSegment(
					integer startId,
					const Vector<Real, 2>& startPosition,
					integer endId,
					const Vector<Real, 2>& endPosition)
					: start_(startId, startPosition)
					, end_(endId, endPosition)
				{
				}

				bool operator<(const StatusSegment& right) const
				{
					const StatusSegment& left = *this;

					// In the following it is important
					// to keep in mind that the comparison
					// is only meaningful inside the overlap
					// detection algorithm. The comparison
					// is used when inserting and removing
					// elements from the status line to traverse
					// the status binary tree.

					// The overlap detection algorithm
					// guarantees that the
					// x-axis projections of the segments
					// overlap.
					// Therefore, one of the start points
					// is in the x-range of the other
					// segment.
					// This start-point is used as the
					// x-coordinate of the vertical line
					// on which the y-coordinates are compared.

					// The information about vertex identity
					// is used explicitly to avoid possible
					// numerical problems.

					// Note that it is not possible to have
					// left.start_.id_ == right.end_.id_ or
					// left.end_.id_ == right.start_.id_
					// This is because at the time of insertion
					// all incident status segments from the left
					// have already been removed.
					// Similarly, the insertions happen from
					// from left to right, so it is not possible
					// to have an existing segment on the right.

					if (left.start_.id_ == right.start_.id_ &&
						left.end_.id_ == right.end_.id_)
					{
						// The segments are the same.
						return false;
					}

					if (left.start_.id_ != right.start_.id_)
					{
						if (left.start_.position_.x() <
							right.start_.position_.x())
						{
							// It is the 'right' segment
							// that has the start point
							// on the shared x-range.

							const Plane<Real, 2> leftPlane(
								left.start_.position_,
								cross(left.end_.position_ -
								left.start_.position_));

							const Real onSide =
								side(right.start_.position_,
								leftPlane);

							if (onSide < 0)
							{
								return false;
							}
							else if (onSide > 0)
							{
								return true;
							}
						}
						else
						{
							// It is the 'left' segment
							// that has the start point
							// on the shared x-range.

							const Plane<Real, 2> rightPlane(
								right.start_.position_,
								cross(right.end_.position_ -
								right.start_.position_));

							const Real onSide =
								side(left.start_.position_,
								rightPlane);

							if (onSide < 0)
							{
								return true;
							}
							else if (onSide > 0)
							{
								return false;
							}
						}
					}

					// If you get here, then
					// three of the points
					// are collinear (one of the end
					// points may not be).

					if (left.end_.id_ != right.end_.id_)
					{
						const Plane<Real, 2> rightPlane(
							right.start_.position_,
							cross(right.end_.position_ - right.start_.position_));

						const Real onSide = side(left.end_.position_,
							rightPlane);

						if (onSide < 0)
						{
							return true;
						}
						else if (onSide > 0)
						{
							return false;
						}

					}

					// If you get here, then
					// the segments are on the same line.
					// See if the start points can give an
					// order.

					if (left.start_ < right.start_)
					{
						return true;
					}
					else if (left.start_ > right.start_)
					{
						return false;
					}

					// If you get here, then the start positions
					// are the same and the segments are on
					// the same line.
					// See if the end points can give an
					// order.

					if (left.end_ < right.end_)
					{
						return true;
					}
					else if (left.end_ > right.end_)
					{
						return false;
					}

					// The segments are absolutely
					// equal.

					return false;
				}

				EventPoint start_;
				EventPoint end_;
			};

			typedef std::set<StatusSegment> StatusContainer;
			typedef typename StatusContainer::iterator
				StatusIterator;
			typedef typename StatusContainer::const_iterator
				ConstStatusIterator;

			typedef ListArray<StatusIterator> IncidenceContainer;
			typedef typename IncidenceContainer::Iterator
				IncidenceIterator;
			typedef typename IncidenceContainer::ConstIterator
				ConstIncidenceIterator;

			typedef ListArray<Integer2> ExidenceContainer;
			typedef ExidenceContainer::Iterator
				ExidenceIterator;
			typedef ExidenceContainer::ConstIterator
				ConstExidenceIterator;
		public:
			OverlapSegments(
				const std::vector<Vector<Real, 2> >& vertex,
				const std::vector<Integer2>& segment);

			bool compute();

		private:
			// Prohibited
			OverlapSegments();
			// Prohibited
			OverlapSegments(const OverlapSegments& that);
			// Prohibited
			OverlapSegments& operator=(const OverlapSegments& that);

			bool removeIncident(const EventPoint& eventPoint);
			bool insertExident(const EventPoint& eventPoint);

			const std::vector<Vector<Real, 2> >& vertex_;
			const std::vector<Integer2>& segment_;

			std::set<EventPoint> event_;
			std::set<StatusSegment> status_;
			IncidenceContainer incidence_;
			ExidenceContainer exidence_;
		};

		template <typename Real>
		OverlapSegments<Real>::OverlapSegments(
			const std::vector<Vector<Real, 2> >& vertex,
			const std::vector<Integer2>& segment)
			: vertex_(vertex)
			, segment_(segment)
			, event_()
			, status_()
			, incidence_()
			, exidence_()
		{
		}

		template <typename Real>
		bool OverlapSegments<Real>::compute()
		{
			// Insert event points into the
			// event queue.

			const integer vertices = vertex_.size();
			for (integer i = 0;i < vertices;++i)
			{
				event_.insert(EventPoint(i, vertex_[i]));
			}

			// For each vertex, gather the set of exident
			// edges. Note incidence edges are not gathered
			// here. Rather, the incident edges are gathered
			// inside the algorithm. At the same time a
			// new segment is inserted into the status line,
			// it is inserted as an incident edge to the
			// end vertex. This guarantees that
			// when an edge is removed from the status
			// line, it also exists there.
			// For example, if there are two segments
			// with the same vertex ids, the segment
			// is only inserted once and removed once.

			const integer segments = segment_.size();

			incidence_.setBuckets(vertices);
			exidence_.setBuckets(vertices);

			for (integer i = 0;i < segments;++i)
			{
				const integer startId = segment_[i][0];
				const integer endId = segment_[i][1];

				const EventPoint start(
					segment_[i][0],
					vertex_[startId]);

				const EventPoint end(
					segment_[i][1],
					vertex_[endId]);

				if (start < end)
				{
					exidence_.push_back(startId,
						Integer2(startId, endId));
				}
				else
				{
					exidence_.push_back(endId,
						Integer2(endId, startId));
				}
			}

			// The main algorithm.

			while (!event_.empty())
			{
				EventPoint eventPoint = *event_.begin();
				event_.erase(event_.begin());

				if (removeIncident(eventPoint))
				{
					return true;
				}
				if (insertExident(eventPoint))
				{
					return true;
				}
			}

			return false;
		}

		template <typename Real>
		bool OverlapSegments<Real>::removeIncident(
			const EventPoint& eventPoint)
		{
			// Remove incident segments
			// from the status.

			IncidenceIterator iter(
				incidence_.begin(eventPoint.id_));
			const IncidenceIterator iterEnd(
				incidence_.end(eventPoint.id_));

			while (iter != iterEnd)
			{
				StatusIterator statusIter(*iter);
				if (statusIter != status_.begin())
				{
					// The segment is not the first...

					StatusIterator next(statusIter);
					++next;
					if (next != status_.end())
					{
						// ... and it is not the last.
						StatusIterator previous(statusIter);
						--previous;

						// So test the neighbours
						// for overlap.

						if (previous->start_.id_ !=
							next->start_.id_ &&
							previous->end_.id_ !=
							next->end_.id_ &&
							overlaps(Segment<Real, 2>(
							previous->start_.position_,
							previous->end_.position_),
							Segment<Real, 2>(
							next->start_.position_,
							next->end_.position_)))
						{
							return true;
						}
					}

				}

				status_.erase(statusIter);

				++iter;
			}

			return false;
		}

		template <typename Real>
		bool OverlapSegments<Real>::insertExident(
			const EventPoint& eventPoint)
		{
			// Add exident segments
			// to the status

			ExidenceIterator iter(
				exidence_.begin(eventPoint.id_));
			const ExidenceIterator iterEnd(
				exidence_.end(eventPoint.id_));

			while (iter != iterEnd)
			{
				const integer startId = (*iter)[0];
				const integer endId = (*iter)[1];

				const Vector<Real, 2>& start =
					vertex_[startId];
				const Vector<Real, 2>& end =
					vertex_[endId];

				const StatusSegment newStatus(
					startId, start,
					endId, end);
				std::pair<StatusIterator, bool> result(
					status_.insert(newStatus));
				if (REPORT(!result.second))
				{
					++iter;
					break;
				}
				StatusIterator statusIter(result.first);

				// Add incidence information to the
				// end vertex.
				incidence_.push_back(endId,
					statusIter);

				if (statusIter != status_.begin())
				{
					// The segment is not the first,
					// so test for intersection with the previous
					// segment.

					StatusIterator previous(statusIter);
					--previous;

					// Segments that share an endpoint
					// are never considered as
					// intersecting.

					if (previous->start_.id_ !=
						statusIter->start_.id_ &&
						previous->end_.id_ !=
						statusIter->end_.id_ &&
						overlaps(
						Segment<Real, 2>(
						previous->start_.position_,
						previous->end_.position_),
						Segment<Real, 2>(
						statusIter->start_.position_,
						statusIter->end_.position_)))
					{
						return true;
					}
				}

				StatusIterator next(statusIter);
				++next;

				if (next != status_.end())
				{
					// The segment is not the last,
					// so test for intersection with the next
					// segment.

					// Segments that share an endpoint
					// are never considered as
					// intersecting.

					if (statusIter->start_.id_ !=
						next->start_.id_ &&
						statusIter->end_.id_ !=
						next->end_.id_ &&
						overlaps(
						Segment<Real, 2>(
						statusIter->start_.position_,
						statusIter->end_.position_),
						Segment<Real, 2>(
						next->start_.position_,
						next->end_.position_)))
					{
						return true;
					}
				}

				++iter;
			}
			return false;
		}

	}

	template <typename Real>
	bool overlaps(
		const std::vector<Vector<Real, 2> >& vertex,
		const std::vector<Integer2>& segment)
	{
		Detail::OverlapSegments<Real> tester(vertex, segment);
		return tester.compute();
	}

	template <typename Real>
	bool overlapsBruteForce(
		const std::vector<Vector<Real, 2> >& vertex,
		const std::vector<Integer2>& segment)
	{
		const integer segments = segment.size();
		for (integer i = 0;i < segments;++i)
		{
			for (integer j = 0;j < segments;++j)
			{
				const Integer2 iSegment = segment[i];
				const Integer2 jSegment = segment[j];

				if (iSegment[0] != jSegment[0] &&
					iSegment[1] != jSegment[1] &&
					iSegment[0] != jSegment[1] &&
					iSegment[1] != jSegment[0])
				{
					if (overlaps(Segment<Real, 2>(
						vertex[iSegment[0]], vertex[iSegment[1]]),
						Segment<Real, 2>(
						vertex[jSegment[0]], vertex[jSegment[1]])))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

}

#endif
