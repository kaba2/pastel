#ifndef PASTEL_TRANSITIVE_CLOSURE_HPP
#define PASTEL_TRANSITIVE_CLOSURE_HPP

#include "pastel/sys/transitive_closure.h"
#include "pastel/sys/unorderedmap.h"

#include <vector>

namespace Pastel
{

	template <
		typename Domain, 
		typename Codomain,
		typename Function,
		typename CodomainOperator, 
		typename ForEachRelated,
		typename ForEachDomain,
		typename Closure_Reporter,
		typename Domain_Hash>
	class TransitiveClosure
	{
	public:
		TransitiveClosure(
			const Codomain& identity_,
			const Function& function_,
			const CodomainOperator& codomainOperator_,
			const ForEachRelated& forEachRelated_,
			const ForEachDomain& forEachDomain_,
			const Closure_Reporter& closureReporter_,
			bool reflexiveClosure_,
			const Domain_Hash& domainHash_)
			: identity(identity_)
			, function(function_)
			, codomainOperator(codomainOperator_)
			, forEachRelated(forEachRelated_)
			, forEachDomain(forEachDomain_)
			, closureReporter(closureReporter_)
			, reflexiveClosure(reflexiveClosure_)
			, domainHash(domainHash_)
		{
		}

		void work()
		{
			auto visitIt = 
				[&](const Domain& x)
			{
				if (!progressSet.count(x))
				{
					// This node has not been visited
					// yet. Visit it now.

					traverse(x);
				}
			};

			forEachDomain(visitIt);

			// Report the closure function, but only
			// for those nodes which are in the domain.
			auto reportIt = 
				[&](const Domain& x)
			{
				auto iter = progressSet.find(x);
				ASSERT(iter != progressSet.end());

				closureReporter(
					iter->first,
					std::move(iter->second.value));
			};

			forEachDomain(reportIt);
		}

	private:
		void traverse(const Domain& x)
		{
			const integer depth = visitedSet.size();

			// Give the node the identity value of the monoid. 
			// If this node does not refer to any nodes (not 
			// even itself), then it will end up having the identity 
			// value.
			auto result = progressSet.insert(
				std::make_pair(x, Progress(depth, identity)));

			// The traverse() function should only
			// be called once for each 'x', so that
			// there should not be an existing entry for
			// the 'x'.
			ASSERT(result.second);
			Progress& xProgress = result.first->second;

			// Push the node to the list of to-be-handled nodes.
			// Essentially, this list stores the elements of the
			// strongly connected components, with each component
			// being an interval in this list. It should be noted
			// though, that if a node does not refer to any node,
			// then it is _not_ a strongly connected component,
			// but still part of this list.
			visitedSet.push_back(x);

			bool isReflexiveAtX = false;

			auto dealWithIt = [&](const Domain& y)
			{
				if (x == y)
				{
					isReflexiveAtX = true;
				}

				// If 'y' has not beed traversed yet...
				if (!progressSet.count(y))
				{
					// ... traverse it now.
					traverse(y);
				}

				const Progress& yProgress = progressSet[y];
				if (yProgress.depth >= 0 && yProgress.depth < xProgress.depth)
				{
					// The depth of the current node should be the 
					// minimum of the depths encountered in children.
					// Negative depth values are used to mark nodes
					// which have already been assigned a strongly
					// connected component.
					xProgress.depth = yProgress.depth;
				}

				// Apply the operator.
				// Note that the current value is passed as rvalue 
				// reference, since we are going to overwrite (ideally, 
				// move assign) it anyway with the result. This avoids 
				// allocating a temporary value. If the node refers to 
				// itself, the added value is the value of the function.
				xProgress.value = codomainOperator(
					std::move(xProgress.value),
					(x == y) ? this->function(x) : yProgress.value);

				// The 'this->function' is needed because of some
				// bug with Visual Studio 2010 SP1 which confuses
				// 'function' member variable with std::function
				// class template.
			};

			forEachRelated(x, dealWithIt);

			// Force a reflexive relation if necessary.
			if (reflexiveClosure && !isReflexiveAtX)
			{
				dealWithIt(x);
			}

			if (xProgress.depth == depth)
			{
				// All of the nodes after 'x' in the 'visitedSet' 
				// belong to the same strongly connected component
				// whose root is 'x'.
				bool ready = false;
				while(!ready)
				{
					const Domain& y = visitedSet.back();
					Progress& yProgress = progressSet[y];

					const integer NeverVisitAgain = -1;
					yProgress.depth = NeverVisitAgain;

					if (y == x)
					{
						ready = true;
					}
					else
					{
						// In the same strongly connected component,
						// the computed values are the same. So
						// simply copy that value.
						xProgress.value = yProgress.value;
					}
					visitedSet.pop_back();
				}
			}
		}

	private:
		class Progress
		{
		public:
			Progress()
				: depth(0)
				, value()
			{
			}

			Progress(
				integer depth_,
				const Codomain& value_)
				: depth(depth_)
				, value(value_)
			{
			}

			Progress(
				integer depth_,
				Codomain&& value_)
				: depth(depth_)
				, value(value_)
			{
			}

			integer depth;
			Codomain value;
		};

		const Codomain& identity;
		const Domain_Hash& domainHash;
		const Function& function;
		const CodomainOperator& codomainOperator;
		const ForEachDomain& forEachDomain;
		const ForEachRelated& forEachRelated;
		const Closure_Reporter& closureReporter;
		bool reflexiveClosure;

		typedef UnorderedMap<Domain, Progress, Domain_Hash> ProgressSet;
		typedef typename ProgressSet::iterator Progress_Iterator;
		typedef typename ProgressSet::iterator Progress_ConstIterator;

		ProgressSet progressSet;
		std::vector<Domain> visitedSet;
	};

	template <
		typename Domain, 
		typename Codomain,
		typename Function,
		typename CodomainOperator, 
		typename ForEachRelated,
		typename ForEachDomain,
		typename Closure_Reporter,
		typename Domain_Hash>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const ForEachRelated& forEachRelated,
		const ForEachDomain& forEachDomain,
		const Closure_Reporter& closureReporter,
		bool reflexiveClosure,
		const Domain_Hash& domainHash)
	{
		TransitiveClosure<Domain, Codomain,
			Function, CodomainOperator,
			ForEachRelated, ForEachDomain, Closure_Reporter, Domain_Hash> algorithm(
				identity,
				function,
				codomainOperator,
				forEachRelated,
				forEachDomain,
				closureReporter,
				reflexiveClosure,
				domainHash);

		algorithm.work();
	}

	template <
		typename Domain, 
		typename Codomain,
		typename Function,
		typename CodomainOperator, 
		typename ForEachRelated,
		typename ForEachDomain,
		typename Closure_Reporter>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const ForEachRelated& forEachRelated,
		const ForEachDomain& forEachDomain,
		const Closure_Reporter& closureReporter,
		bool reflexiveClosure)
	{
		return Pastel::transitiveClosure<Domain, Codomain>(
			identity,
			function,
			codomainOperator,
			forEachRelated,
			forEachDomain,
			closureReporter,
			reflexiveClosure,
			std::hash<Domain>());
	}

}

#endif
