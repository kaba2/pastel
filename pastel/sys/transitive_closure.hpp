#ifndef PASTELSYS_TRANSITIVE_CLOSURE_HPP
#define PASTELSYS_TRANSITIVE_CLOSURE_HPP

#include "pastel/sys/transitive_closure.h"

#include <unordered_map>
#include <vector>

namespace Pastel
{

	template <
		typename Domain, 
		typename Codomain,
		typename ForEachDomain,
		typename ForEachRelated,
		typename Function,
		typename CodomainOperator, 
		typename Closure_Reporter,
		typename Domain_Hash>
	class TransitiveClosure
	{
	public:
		TransitiveClosure(
			const Codomain& identity_,
			const ForEachDomain& forEachDomain_,
			const ForEachRelated& forEachRelated_,
			const Function& function_,
			const CodomainOperator& codomainOperator_,
			const Closure_Reporter& report_,
			bool reflexiveClosure_,
			const Domain_Hash& domainHash_)
			: identity(identity_)
			, forEachDomain(forEachDomain_)
			, forEachRelated(forEachRelated_)
			, function(function_)
			, codomainOperator(codomainOperator_)
			, report(report_)
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

				report(
					iter->first,
					std::move(iter->second.value));
			};

			forEachDomain(reportIt);
		}

	private:
		void traverse(const Domain& x)
		{
			const integer ClosureValueReady = -1;
			const integer depth = workSet.size();

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
			// being an interval in this list. However, there are
			// also singular nodes in this list which are not
			// strongly connected components (the node must be
			// related to itself to be a strongly connected 
			// component).
			workSet.push_back(x);

			auto dealWithIt = [&](const Domain& y)
			{
				if (x == y)
				{
					// The node x is related to itself.
					// Add the function-value to the
					// closure-value.
					xProgress.value = codomainOperator(
						std::move(xProgress.value), 
						this->function(x));
					xProgress.reflexive = true;
					return;
				}

				// If 'y' has not beed traversed yet...
				if (!progressSet.count(y))
				{
					// ... traverse it now.
					traverse(y);
				}

				const Progress& yProgress = progressSet[y];
				if (yProgress.depth >= 0)
				{
					// After finishing visiting the related nodes,
					// the depth of node x is the minimum of the depths 
					// encountered in children.
					if (yProgress.depth < xProgress.depth)
					{
						xProgress.depth = yProgress.depth;
					}
				}
				else
				{
					// If the node y has a negative depth value, then
					// this means that its closure-value has already
					// been computed.

					ASSERT(x != y);

					// Add the closure-value of node y to the 
					// closure-value of node x.
					xProgress.value = codomainOperator(
						std::move(xProgress.value), 
						yProgress.value);

					if (!yProgress.reflexive)
					{
						// If the function-value of node y is not
						// part of the closure-value of node y, then 
						// add it now to the closure-value of node x.
						xProgress.value = codomainOperator(
							std::move(xProgress.value), 
							this->function(y));
					}
				}
			};

			forEachRelated(x, dealWithIt);

			// Force a reflexive relation if necessary.
			if (reflexiveClosure && !xProgress.reflexive)
			{
				dealWithIt(x);
			}

			if (xProgress.depth == depth)
			{
				// If the minimum encountered depth-value is the
				// same as the depth-value of node x, then all the
				// nodes after 'x' in the 'workSet' belong to the 
				// same strongly connected component as node x.

				bool moreThanOneElement =
					(workSet.back() != x);
				
				if (moreThanOneElement)
				{
					// We have a strongly connected component
					// with at least two elements. These elements
					// all have the same closure-value, which is
					// the combination of the functions-values of
					// the elements. Compute this shared closure-value
					// incrementally.

					auto iter = workSet.end();
					do
					{
						--iter;
						const Domain& y = *iter;
						Progress& yProgress = progressSet[y];

						if (!yProgress.reflexive)
						{
							// If a node in the component is not reflexive,
							// then make it so.
							yProgress.value = codomainOperator(
								std::move(yProgress.value), function(y));
							yProgress.reflexive = true;
						}

						if (x != y)
						{
							// Add the closure-value of node y to the 
							// closure-value of node x.
							xProgress.value = codomainOperator(
								std::move(xProgress.value), yProgress.value);
						}
					}
					while(*iter != x);

					// Note that the closure-values are not correct yet.
					// They need to copied from node x to the other nodes.
					// This is done next.
				}

				bool ready = false;
				while(!ready)
				{
					const Domain& y = workSet.back();
					Progress& yProgress = progressSet[y];

					// Mark the node as having a fully-computed
					// closure-value.
					yProgress.depth = ClosureValueReady;

					if (y == x)
					{
						// No need to copy the closure-value of 
						// node x to itself.
						ready = true;
					}
					else
					{
						// Copy the closure-value inside the
						// strongly connected component.
						yProgress.value = xProgress.value;
					}

					// Remove the node from the work-set.
					workSet.pop_back();
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
				, reflexive(false)
			{
			}

			Progress(
				integer depth_,
				const Codomain& value_)
				: depth(depth_)
				, value(value_)
				, reflexive(false)
			{
			}

			Progress(
				integer depth_,
				Codomain&& value_)
				: depth(depth_)
				, value(value_)
				, reflexive(false)
			{
			}

			integer depth;
			Codomain value;
			bool reflexive;
		};

		const Codomain& identity;
		const ForEachDomain& forEachDomain;
		const ForEachRelated& forEachRelated;
		const Domain_Hash& domainHash;
		const Function& function;
		const CodomainOperator& codomainOperator;
		const Closure_Reporter& report;
		bool reflexiveClosure;

		typedef std::unordered_map<Domain, Progress, Domain_Hash> ProgressSet;
		typedef typename ProgressSet::iterator Progress_Iterator;
		typedef typename ProgressSet::iterator Progress_ConstIterator;

		ProgressSet progressSet;
		std::vector<Domain> workSet;
	};

	template <
		typename Domain, 
		typename Codomain,
		typename ForEachDomain,
		typename ForEachRelated,
		typename Function,
		typename CodomainOperator, 
		typename Closure_Reporter,
		typename Domain_Hash>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const ForEachDomain& forEachDomain,
		const ForEachRelated& forEachRelated,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const Closure_Reporter& report,
		bool reflexiveClosure,
		const Domain_Hash& domainHash)
	{
		TransitiveClosure<Domain, Codomain, 
			ForEachDomain, ForEachRelated, 
			Function, CodomainOperator,
			Closure_Reporter, Domain_Hash> algorithm(
				identity,
				forEachDomain,
				forEachRelated,
				function,
				codomainOperator,
				report,
				reflexiveClosure,
				domainHash);

		algorithm.work();
	}

	template <
		typename Domain, 
		typename Codomain,
		typename ForEachDomain,
		typename ForEachRelated,
		typename Function,
		typename CodomainOperator, 
		typename Closure_Reporter>
	void transitiveClosure(
		const PASTEL_NO_DEDUCTION(Codomain)& identity,
		const ForEachDomain& forEachDomain,
		const ForEachRelated& forEachRelated,
		const Function& function,
		const CodomainOperator& codomainOperator,
		const Closure_Reporter& report,
		bool reflexiveClosure)
	{
		return Pastel::transitiveClosure<Domain, Codomain>(
			identity,
			forEachDomain,
			forEachRelated,
			function,
			codomainOperator,
			report,
			reflexiveClosure,
			std::hash<Domain>());
	}

}

#endif
