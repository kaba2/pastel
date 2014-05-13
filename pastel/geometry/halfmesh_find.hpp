#ifndef PASTELGEOMETRY_HALFMESH_FIND_HPP
#define PASTELGEOMETRY_HALFMESH_FIND_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::findHalf(
		const Vertex_ConstIterator& from, 
		const Vertex_ConstIterator& to) const
		-> Half_ConstIterator
	{
		PENSURE(!from.empty());
		PENSURE(!to.empty());

		if (from.isolated() || to.isolated())
		{
			return Half_ConstIterator();
		}

		Half_ConstIterator begin = from->half();
		Half_ConstIterator current = begin;

		do
		{
			if (current->destination() == to)
			{
				return current;
			}

			current = current->rotateNext();
		}
		while (current != begin);

		return Half_ConstIterator();
	}

	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::findFreeIncident(
		const Vertex_ConstIterator& vertex) const
		-> Half_ConstIterator
	{
		ASSERT(!vertex->empty());
		ASSERT(!vertex->isolated());

		Half_ConstIterator begin = vertex->half()->pair();
		Half_ConstIterator current = begin;
		do
		{
			if (current->left()->empty())
			{
				return current;
			}
			current = current->next()->pair();
		}
		while (current != begin);

		return Half_ConstIterator();
	}

	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::findFreeIncident(
		const Vertex_ConstIterator& vertex, 
		const Half_ConstIterator& startingFrom,
		const Half_ConstIterator& andBefore) const
		-> Half_ConstIterator
	{
		ASSERT(!vertex->empty());
		ASSERT(!vertex->isolated());
		ASSERT(!startingFrom->empty());
		ASSERT(startingFrom->destination() == vertex);
		ASSERT(!andBefore->empty());
		ASSERT(andBefore->destination() == vertex);

		if (andBefore == startingFrom)
		{
			return Half_ConstIterator();
		}

		Half_ConstIterator current = startingFrom;
		do
		{
			if (current->left()->empty())
			{
				return current;
			}
			current = current->next()->pair();
		}
		while (current != andBefore);

		return Half_ConstIterator();
	}

}

#endif
