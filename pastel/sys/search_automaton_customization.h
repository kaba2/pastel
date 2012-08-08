#ifndef PASTEL_SEARCH_AUTOMATON_CUSTOMIZATION_H
#define PASTEL_SEARCH_AUTOMATON_CUSTOMIZATION_H

#include "pastel/sys/automaton_concepts.h"

#include "pastel/sys/automaton.h"

#include <unordered_set>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData>
	class Search_Automaton_Customization
		: public Automaton_Concepts::Customization<Symbol, StateData, TransitionData>
	{
	private:
		typedef Automaton_Fwd<Symbol, StateData, TransitionData> Fwd;
		
		PASTEL_FWD(State_Iterator);
		PASTEL_FWD(State_ConstIterator);
		PASTEL_FWD(Transition_Iterator);
		PASTEL_FWD(Transition_ConstIterator);

		typedef Automaton<Symbol, StateData, TransitionData,
			Search_Automaton_Customization> Automaton;

	public:
		typedef std::unordered_map<
			State_ConstIterator, Transition_ConstIterator,
			IteratorAddress_Hash>
			BranchMap;
		typedef typename BranchMap::iterator
			Branch_Iterator;
		typedef typename BranchMap::const_iterator
			Branch_ConstIterator;

		class StateSymbol
		{
		public:
			StateSymbol(
				State_ConstIterator state_,
				Symbol symbol_)
				: state(state_)
				, symbol(symbol_)
			{
			}
			
			bool operator==(const StateSymbol& that) const
			{
				return state == that.state &&
					symbol == that.symbol;
			}

			bool operator!=(const StateSymbol& that) const
			{
				return !(*this == that);
			}

			State_ConstIterator state;
			Symbol symbol;
		};

		class StateSymbol_Hash
		{
		public:
			Pastel::hash_integer operator()(
				const StateSymbol& transition) const
			{
				return Pastel::combineHash(
					Pastel::computeHash(&*transition.state),
					Pastel::computeHash(transition.symbol));
			}
		};

	private:
		typedef std::unordered_map<StateSymbol, 
			BranchMap, StateSymbol_Hash> Symbol_BranchMap_Map;
		typedef typename Symbol_BranchMap_Map::iterator
			Symbol_BranchMap_Iterator;
		typedef typename Symbol_BranchMap_Map::const_iterator
			Symbol_BranchMap_ConstIterator;

	public:		
		// The new member functions.

		//! Returns the set of branches from 'state' with 'symbol'.
		/*!
		Time complexity:
		O(1) on average

		Exception safety:
		nothrow

		A set of transitions which share the from-state and the
		symbol is called a branch-set (because if there is more
		than one branch, then the computation in an NFA branches).
		*/
		const BranchMap& branchMap(
			const State_ConstIterator& state,
			const Symbol& symbol) const
		{
			Symbol_BranchMap_ConstIterator branchMap =
				branchMapMap_.find(StateSymbol(state, symbol));
			if (branchMap == branchMapMap_.cend())
			{
				return emptyBranchMap_;
			}

			return branchMap->second;
		}

		//! Returns whether there is a transition from 'state' with 'symbol'.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		bool existsTransition(
			const State_ConstIterator& state,
			const Symbol& symbol) const
		{
			const BranchMap& branch =
				branchMap(state, symbol);
			return !branch.empty();
		}

		Transition_ConstIterator findTransition(
			const State_ConstIterator& state,
			const Symbol& symbol) const
		{
			const BranchMap& branch =
				branchMap(state, symbol);

			if (branch.empty())
			{
				return ((Automaton&)*this).cTransitionEnd();
			}

			return branch.cbegin()->second;
		}

	protected:
		//! Creates an empty customization.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Search_Automaton_Customization()
			: branchMapMap_()
			, emptyBranchMap_()
		{
		}

		//! Swaps two customizations.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(Search_Automaton_Customization& that) 
		{
			branchMapMap_.swap(that.branchMapMap_);
			emptyBranchMap_.swap(that.emptyBranchMap_);
		}

		//! Removes all transitions from the search-set.
		/*!
		Time complexity:
		O(m)
		where
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void onClear() 
		{
			branchMapMap_.clear();
		}

		//! Removes all transitions from the search-set.
		/*!
		Time complexity:
		O(m)
		where
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void onClearTransitions() 
		{
			branchMapMap_.clear();
		}

		//! Adds a transition into the search-set.
		/*!
		Time complexity: O(1) on average
		Exception safety: strong
		*/
		void onAddTransition(
			const Transition_ConstIterator& transition)	
		{
			// See if the branch-set for the transition
			// symbol already exists.
			Symbol_BranchMap_Iterator branchMap =
				branchMapMap_.find(
				StateSymbol(transition->from(), transition->symbol()));

			if (branchMap == branchMapMap_.end())
			{
				// The branch-set does not exist,
				// create it now.
				branchMap = branchMapMap_.insert(
					std::make_pair(StateSymbol(transition->from(), 
					transition->symbol()),
					BranchMap())).first;
			}
			
			try
			{
				// Add the transition into the branch set.
				branchMap->second.insert(
					std::make_pair(transition->to(), transition));
			}
			catch(...)
			{
				if (branchMap->second.empty())
				{
					branchMapMap_.erase(branchMap);
				}
				throw;
			}
		}

		//! Removes a transition from the search-set.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		void onRemoveTransition(
			const Transition_ConstIterator& transition)	
		{
			Symbol_BranchMap_Iterator branchMap =
				branchMapMap_.find(StateSymbol(
				transition->from(), transition->symbol()));
			ASSERT(branchMap != branchMapMap_.end());

			branchMap->second.erase(transition);
			if (branchMap->second.empty())
			{
				branchMapMap_.erase(branchMap);
			}
		}

		//! Merges two customizations.
		/*!
		Time complexity:
		O(m + m')
		where
		m is the number of transitions in this automaton, and
		m' is the number of transitions in the other automaton.

		Exception safety:
		strong
		*/
		void onMerge(Automaton& that) 
		{
			Symbol_BranchMap_Map branchMapMap;
			
			branchMapMap.insert(
				branchMapMap_.begin(),
				branchMapMap_.end());
			
			branchMapMap.insert(
				that.branchMapMap_.begin(),
				that.branchMapMap_.end());
			
			branchMapMap_.swap(branchMapMap);
		}

		//! Returns whether to add the transition into the automaton.
		/*!
		Exception safety: nothrow
		*/
		bool canAddTransition(
			const State_ConstIterator& fromState,
			const Symbol& symbol,
			const State_ConstIterator& toState) 
		{
			const BranchMap& branch =
				branchMap(state, symbol);
			
			return !branch.count(toState);
		}

	private:
		Search_Automaton_Customization(
			Search_Automaton_Customization&& that) PASTEL_DELETE;
		Search_Automaton_Customization(
			const Search_Automaton_Customization& that) PASTEL_DELETE;
		Search_Automaton_Customization& operator=(
			Search_Automaton_Customization that) PASTEL_DELETE;

		Symbol_BranchMap_Map branchMapMap_;
		BranchMap emptyBranchMap_;
	};

}

#endif
