// Description: Finite-state automaton

#ifndef PASTELSYS_AUTOMATON_H
#define PASTELSYS_AUTOMATON_H

#include "pastel/sys/automaton/automaton_concepts.h"
#include "pastel/sys/automaton/automaton_fwd.h"
#include "pastel/sys/optional/optional.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"
#include "pastel/sys/incidence_graph.h"

#include <unordered_map>

namespace Pastel
{

	//! Finite-state automaton
	template <
		typename Symbol, 
		typename StateData_ = Empty, 
		typename TransitionData_ = Empty,
		typename Customization = No_Automaton_Customization<Symbol, StateData_, TransitionData_>>
	class Automaton
		: public Customization
	{
	private:
		using Fwd = Automaton_Fwd<Symbol, StateData_, TransitionData_>;

		PASTEL_FWD(StateLabel);
		PASTEL_FWD(TransitionLabel);
		PASTEL_FWD(Graph);

		PASTEL_FWD(Actual_Branch_Iterator);
		PASTEL_FWD(Actual_Branch_ConstIterator);

		PASTEL_FWD(Symbol_BranchMap_Map);
		PASTEL_FWD(Symbol_BranchMap_Iterator);
		PASTEL_FWD(Symbol_BranchMap_ConstIterator);
		PASTEL_FWD(StateSymbol);
		PASTEL_FWD(StateSymbol_Hash);

	public:
		PASTEL_FWD(State_Iterator);
		PASTEL_FWD(State_ConstIterator);
		PASTEL_FWD(StateData);

		PASTEL_FWD(Transition_Iterator);
		PASTEL_FWD(Transition_ConstIterator);
		PASTEL_FWD(TransitionData);
		
		PASTEL_FWD(Incidence_Iterator);
		PASTEL_FWD(Incidence_ConstIterator);

		PASTEL_FWD(StartSet);
		PASTEL_FWD(Start_Iterator);
		PASTEL_FWD(Start_ConstIterator);
		
		PASTEL_FWD(FinalSet);
		PASTEL_FWD(Final_Iterator);
		PASTEL_FWD(Final_ConstIterator);

		PASTEL_FWD(BranchMap);
		PASTEL_FWD(Branch_Iterator);
		PASTEL_FWD(Branch_ConstIterator);

		//! Constructs an empty automaton.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Automaton()
			: graph_()
			, startSet_()
			, finalSet_()
			, epsilonTransitions_(0)
			, ambiguousTransitions_(0)
		{
		}

		//! Copy-constructs from another automaton.
		/*!
		Time complexity: 
		O(that.states() + that.transitions())

		Exception safety: 
		strong
		*/
		Automaton(const Automaton& that)
			: graph_()
			, startSet_()
			, finalSet_()
			, epsilonTransitions_(0)
			, ambiguousTransitions_(0)
		{
			// This will map states from 'that' automaton
			// to this automaton.
			std::unordered_map<
				State_ConstIterator,
				State_ConstIterator,
				IteratorAddress_Hash> stateMap;

			// Create the states and the state-mapping.
			for (auto state = that.cStateBegin();
				state != that.cStateEnd();
				++state)
			{
				stateMap[state] = addState(*state);
			}
			
			// Create the transitions.
			for (auto transition = that.cTransitionBegin();
				transition != that.cTransitionEnd();
				++transition)
			{
				addTransition(
					stateMap[transition->from()],
					transition->symbol(),
					stateMap[transition->to()],
					*transition);
			}

			// Add the start states.
			std::for_each(
				that.cStartBegin(),
				that.cStartEnd(),
				[&](const State_ConstIterator& state)
			{
				addStart(stateMap[state]);
			});

			// Add the final states.
			std::for_each(
				that.cFinalBegin(),
				that.cFinalEnd(),
				[&](const State_ConstIterator& state)
			{
				addFinal(stateMap[state]);
			});
		}

		//! Move-constructs from another automaton.
		/*!
		Time complexity: 
		O(1)

		Exception safety: 
		strong
		*/
		Automaton(Automaton&& that)
			: graph_()
			, startSet_()
			, finalSet_()
			, epsilonTransitions_(0)
			, ambiguousTransitions_(0)
		{
			swap(that);
		}

		//! Move-assigns from another automaton.
		/*!
		Time complexity: 
		Move/copy-construction

		Exception safety: 
		Move/copy-construction
		*/
		Automaton& operator=(Automaton that)
		{
			swap(that);
			return *this;
		}

		//! Removes all states and transitions.
		/*!
		Time complexity:
		O(n + m) + customization,
		where 
		n is the number of states, and
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void clear()
		{
			this->onClear();

			graph_.clear();
			startSet_.clear();
			finalSet_.clear();
			branchMapMap_.clear();
			epsilonTransitions_ = 0;
			ambiguousTransitions_ = 0;
		}

		//! Removes all transitions.
		/*!
		Time complexity:
		O(m) + customization,
		where
		m is the number of transitions.

		Exception safety:
		nothrow
		*/
		void clearTransitions()
		{
			this->onClearTransitions();

			graph_.clearEdges();
			branchMapMap_.clear();
			epsilonTransitions_ = 0;
			ambiguousTransitions_ = 0;
		}

		//! Removes all start-state marks.
		/*!
		Time complexity:
		O(k) + customization,
		where
		k is the number of start-states.

		Exception safety:
		nothrow
		*/
		void clearStart()
		{
			this->onClearStart();

			while(startStates() > 0)
			{
				removeStart(startSet_.front());
			}
		}

		//! Removes all final-state marks.
		/*!
		Time complexity:
		O(k) + customization
		where
		k is the number of start-states.

		Exception safety:
		nothrow
		*/
		void clearFinal()
		{
			this->onClearFinal();

			while(finalStates() > 0)
			{
				removeFinal(finalSet_.front());
			}
		}

		//! Swaps two automata.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		void swap(Automaton& that)
		{
			using std::swap;

			Customization::swap((Customization&)that);
			graph_.swap(that.graph_);
			startSet_.swap(that.startSet_);
			finalSet_.swap(that.finalSet_);
			branchMapMap_.swap(that.branchMapMap_);
			emptyBranchMap_.swap(that.emptyBranchMap_);
			swap(epsilonTransitions_, that.epsilonTransitions_);
			swap(ambiguousTransitions_, that.ambiguousTransitions_);
		}

		// States

		//! Adds a new state.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		strong + customization
		*/
		State_Iterator addState(StateData stateData = StateData())
		{
			State_Iterator state = graph_.insertVertex(
				StateLabel(std::move(stateData)));

			try
			{
				this->onAddState(state);
			}
			catch(...)
			{
				graph_.removeVertex(state);
				throw;
			}

			return state;
		}

		//! Removes a state.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		State_Iterator removeState(
			const State_ConstIterator& state)
		{
			this->onRemoveState(state);

			// Remove all transitions that are 
			// incoming or outgoing at this vertex.
			while(state->allEdges() > 0)
			{
				removeTransition(
					state->cbegin()->edge());
			}

			if (state->final())
			{
				// Remove designation of a final state.
				removeFinal(state);
			}

			if (state->start())
			{
				// Remove designation of a start state.
				removeStart(state);
			}

			// Remove the vertex.
			return graph_.removeVertex(state);
		}

		//! Removes constness from a state-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator cast(
			const State_ConstIterator& state)
		{
			return graph_.cast(state);
		}

		// Start states

		//! Adds the given state to the start states.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		strong + customization
		*/
		void addStart(
			const State_ConstIterator& state)
		{
			if (state->data().start())
			{
				return;
			}
			
			Start_Iterator start = startSet_.insert(
				startSet_.end(), state);

			cast(state)->data().startPosition_ = start; 
			cast(state)->data().setStart(true);

			try
			{
				this->onAddStart(state);
			}
			catch(...)
			{
				cast(state)->data().startPosition_ = startSet_.end(); 
				cast(state)->data().setStart(false);
				startSet_.erase(start);
				throw;
			}
		}
		
		//! Removes the given state from the start-states.
		/*!
		Time complexity:
		O(1) + customization

		Exception safety:
		nothrow
		*/
		Start_Iterator removeStart(
			const State_ConstIterator& state)
		{
			if (!state->start())
			{
				return state->startPosition_;
			}
			
			this->onRemoveStart(state);

			Start_Iterator next =
				startSet_.erase(state->startPosition_);
			cast(state)->data().setStart(false);

			return next;
		}

		//! Returns the first iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_Iterator startBegin()
		{
			return startSet_.begin();
		}

		//! Returns the first iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_ConstIterator cStartBegin() const
		{
			return ((Automaton&)*this).startSet_.begin();
		}

		//! Returns the end-iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_Iterator startEnd()
		{
			return startSet_.end();
		}

		//! Returns the end-iterator of the start-states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Start_ConstIterator cStartEnd() const
		{
			return ((Automaton&)*this).startSet_.end();
		}

		//! Returns the number of start states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer startStates() const
		{
			return startSet_.size();
		}

		// Final states

		//! Makes the given state final.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		strong + customization
		*/
		void addFinal(
			const State_ConstIterator& state)
		{
			if (state->data().final())
			{
				return;
			}

			Final_Iterator final = finalSet_.insert(
				finalSet_.end(), state);

			cast(state)->data().finalPosition_ = final;
			cast(state)->data().setFinal(true);

			try
			{
				this->onAddFinal(state);
			}
			catch(...)
			{
				cast(state)->data().finalPosition_ = finalSet_.end();
				cast(state)->data().setFinal(false);
				finalSet_.erase(final);
				throw;
			}
		}
		
		//! Removes finality from the given state.
		/*!
		Time complexity: 
		O(1) + customization
		
		Exception safety: 
		nothrow
		*/
		Final_Iterator removeFinal(
			const State_ConstIterator& state)
		{
			if (!state->final())
			{
				return state->data().finalPosition_;
			}

			this->onRemoveFinal(state);

			Final_Iterator next = 
				finalSet_.erase(state->data().finalPosition_);
			cast(state)->data().finalPosition_ = finalSet_.end();
			cast(state)->data().setFinal(false);

			return next;
		}

		//! Returns the first iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_Iterator finalBegin()
		{
			return finalSet_.begin();
		}

		//! Returns the first iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_ConstIterator cFinalBegin() const
		{
			return ((Automaton&)*this).finalSet_.begin();
		}

		//! Returns the end-iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_Iterator finalEnd()
		{
			return finalSet_.end();
		}

		//! Returns the end-iterator of the final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Final_ConstIterator cFinalEnd() const
		{
			return ((Automaton&)*this).finalSet_.end();
		}

		//! Returns the number of final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer finalStates() const
		{
			return finalSet_.size();
		}

		// All states

		//! Returns the first iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator stateBegin()
		{
			return graph_.vertexBegin();
		}

		//! Returns the first iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_ConstIterator cStateBegin() const
		{
			return graph_.cVertexBegin();
		}

		//! Returns the end-iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_Iterator stateEnd()
		{
			return graph_.vertexEnd();
		}

		//! Returns the end-iterator of the state-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		State_ConstIterator cStateEnd() const
		{
			return graph_.cVertexEnd();
		}

		//! Returns the number of states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer states() const
		{
			return graph_.vertices();
		}

		// Transitions

		//! Adds a new transition.
		/*!
		Time complexity: 
		O(1) + customization

		Exception safety: 
		strong + customization
		*/
		Transition_Iterator addTransition(
			const State_ConstIterator& fromState,
			Optional<Symbol> symbol,
			const State_ConstIterator& toState,
			TransitionData transitionData = TransitionData())
		{
			if (!this->canAddTransition(fromState, symbol, toState))
			{
				return transitionEnd();
			}

			integer rollback = 0;

			// See if the branch-set for the transition
			// symbol already exists.
			Symbol_BranchMap_Iterator branchMap =
				branchMapMap_.find(
				StateSymbol(fromState, symbol));
			Actual_Branch_Iterator branch;

			bool ambiguous = false;

			if (branchMap == branchMapMap_.end())
			{
				// The branch-set does not exist,
				// create it now.
				branchMap = branchMapMap_.insert(
					std::make_pair(StateSymbol(fromState, symbol),
					BranchMap())).first;
			}
			else
			{
				// The branch-set already exists.
				ambiguous = true;

				// See if the branch is already there.
				branch = branchMap->second.find(toState);
				if (branch != branchMap->second.cend())
				{
					// The transition is already present in the
					// automaton. Do nothing.
					return cast(branch->second);
				}
			}
			++rollback;

			Transition_Iterator transition;

			try
			{
				// Add the transition into the graph.
				transition = graph_.insertEdge(
					fromState, toState, 
					TransitionLabel(std::move(symbol), std::move(transitionData)));
				++rollback;

				// Add the transition into the branch set.
				branch = branchMap->second.insert(
					std::make_pair(toState, transition)).first;
				++rollback;

				if (symbol.empty())
				{
					// Update the epsilon-transition counter.
					++epsilonTransitions_;
				}

				if (ambiguous)
				{
					// Update the non-deterministic-transition
					// counter.
					++ambiguousTransitions_;
				}

				// Call the customization.
				this->onAddTransition(transition);
			}
			catch(...)
			{
				switch(rollback)
				{
				case 3:
					branchMap->second.erase(branch);
					if (symbol.empty())
					{
						--epsilonTransitions_;
					}
					if (ambiguous)
					{
						--ambiguousTransitions_;
					}
					// Fall-through
				case 2:
					graph_.removeEdge(transition);
					// Fall-through
				case 1:
					if (branchMap->second.empty())
					{
						branchMapMap_.erase(branchMap);
					}
					break;
				};
				throw;
			}

			return transition;
		}

		//! Removes a transition.
		/*!
		Time complexity: 
		O(1) + customization

		Exception safety: 
		nothrow
		*/
		Transition_Iterator removeTransition(
			const Transition_ConstIterator& transition)
		{
			this->onRemoveTransition(transition);

			Symbol_BranchMap_Iterator branchMap =
				branchMapMap_.find(StateSymbol(
				transition->from(), transition->symbol()));
			ASSERT(branchMap != branchMapMap_.end());

			if (transition->symbol().empty())
			{
				--epsilonTransitions_;
			}

			branchMap->second.erase(transition);

			if (branchMap->second.size() == 1)
			{
				--ambiguousTransitions_;
			}

			if (branchMap->second.empty())
			{
				branchMapMap_.erase(branchMap);
			}

			return graph_.removeEdge(transition);
		}

		//! Returns the first iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionBegin()
		{
			return graph_.edgeBegin();
		}

		//! Returns the first iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionBegin() const
		{
			return graph_.cEdgeBegin();
		}

		//! Returns the end-iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator transitionEnd()
		{
			return graph_.edgeEnd();
		}

		//! Returns the end-iterator of the transition-set.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_ConstIterator cTransitionEnd() const
		{
			return graph_.cEdgeEnd();
		}

		//! Returns the first iterator of the outgoing incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_Iterator outgoingBegin(
			const State_ConstIterator& state)
		{
			return state->outgoingBegin();
		}

		//! Returns the first iterator of the outgoing incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_ConstIterator cOutgoingBegin(
			const State_ConstIterator& state) const
		{
			return state->cOutgoingBegin();
		}

		//! Returns the end-iterator of the outgoing incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_Iterator outgoingEnd(
			const State_ConstIterator& state)
		{
			return state->outgoingEnd();
		}

		//! Returns the end-iterator of the outgoing incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_ConstIterator cOutgoingEnd(
			const State_ConstIterator& state) const
		{
			return state->cOutgoingEnd();
		}

		//! Returns the first iterator of the incoming incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_Iterator incomingBegin(
			const State_ConstIterator& state)
		{
			return state->incomingBegin();
		}

		//! Returns the first iterator of the incoming incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_ConstIterator cIncomingBegin(
			const State_ConstIterator& state) const
		{
			return state->cIncomingBegin();
		}

		//! Returns the end-iterator of the incoming incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_Iterator incomingEnd(
			const State_ConstIterator& state)
		{
			return state->incomingEnd();
		}

		//! Returns the end-iterator of the incoming incidences of the state.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Incidence_ConstIterator cIncomingEnd(
			const State_ConstIterator& state) const
		{
			return state->cIncomingEnd();
		}

		//! Removes constness from a transition-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Transition_Iterator cast(
			const Transition_ConstIterator& transition)
		{
			return graph_.cast(transition);
		}

		//! Returns the number of transitions.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer transitions() const
		{
			return graph_.edges();
		}

		// Global operations

		//! Merges two automata together.
		/*!
		Time complexity: 
		O(transitions() + that.transitions()) + customization

		Exception safety: 
		strong + customization
		*/
		void merge(Automaton& that)
		{
			this->onMerge(that);

			Symbol_BranchMap_Map branchMapMap;
			
			branchMapMap.insert(
				branchMapMap_.begin(),
				branchMapMap_.end());
			
			branchMapMap.insert(
				that.branchMapMap_.begin(),
				that.branchMapMap_.end());
			
			branchMapMap_.swap(branchMapMap);

			// Bring in the graph.
			graph_.merge(that.graph_);
			
			// Bring in the start states.
			startSet_.splice(
				startSet_.cend(),
				that.startSet_);

			// Bring in the final states.
			finalSet_.splice(
				finalSet_.cend(),
				that.finalSet_);
		}

		//! Returns whether there are both start and final states.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool useful() const
		{
			return startStates() > 0 &&
				finalStates() > 0;
		}

		// Branch-sets
		
		ranges::subrange<Branch_ConstIterator> cBranchRange(
			const State_ConstIterator& state,
			const Optional<Symbol>& symbol) const
		{
			BranchMap& map = 
				(BranchMap&)branchMap(state, symbol);

			return range(
				Branch_ConstIterator(map.begin()),
				Branch_ConstIterator(map.end()));
		}

		//! Returns whether there is a transition from 'state' with 'symbol'.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		bool existsTransition(
			const State_ConstIterator& fromState,
			const Optional<Symbol>& symbol) const
		{
			return findTransition(fromState, symbol) != 
				cTransitionEnd();
		}

		//! Returns whether the given transition exists.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		bool existsTransition(
			const State_ConstIterator& fromState,
			const Optional<Symbol>& symbol,
			const State_ConstIterator& toState) const
		{
			return findTransition(fromState, symbol, toState) != 
				cTransitionEnd();
		}

		//! Returns some transition from 'state' with 'symbol'.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		Transition_ConstIterator findTransition(
			const State_ConstIterator& fromState,
			const Optional<Symbol>& symbol) const
		{
			const BranchMap& branch =
				branchMap(fromState, symbol);

			if (branch.empty())
			{
				return cTransitionEnd();
			}

			return branch.cbegin()->second;
		}

		//! Returns the given transition if it exists.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		Transition_ConstIterator findTransition(
			const State_ConstIterator& fromState,
			const Optional<Symbol>& symbol,
			const State_ConstIterator& toState) const
		{
			const BranchMap& map =
				branchMap(fromState, symbol);

			Branch_ConstIterator branch = 
				map.find(toState);
			if (branch == map.cend())
			{
				return cTransitionEnd();
			}

			return branch->second;
		}
		
		//! Returns the number of epsilon-transitions.

		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer epsilonTransitions() const
		{
			return epsilonTransitions_;
		}

		//! Returns the number non-deterministic transitions.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer ambiguousTransitions() const
		{
			return ambiguousTransitions_;
		}

		//! Returns whether the automaton is deterministic.
		/*!
		The automaton is deterministic if and only if
		it has at most one start-state, has no epsilon-transitions,
		and has at most one transition from a given state with
		a given symbol.

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool deterministic() const
		{
			return startStates() <= 1 &&
				epsilonTransitions() == 0 &&
				ambiguousTransitions() == 0;
		}

	private:
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
			const Optional<Symbol>& symbol) const
		{
			Symbol_BranchMap_ConstIterator branchMap =
				branchMapMap_.find(StateSymbol(state, symbol));
			if (branchMap == branchMapMap_.cend())
			{
				return emptyBranchMap_;
			}

			return branchMap->second;
		}

		//! The underlying graph.
		Graph graph_;
		
		//! The set of start states.
		StartSet startSet_;

		//! The set of final states.
		FinalSet finalSet_;

		Symbol_BranchMap_Map branchMapMap_;
		BranchMap emptyBranchMap_;

		//! The number of epsilon-transitions.
		integer epsilonTransitions_;

		//! The number of non-deterministic transitions.
		integer ambiguousTransitions_;
	};

}

#include <ostream>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization>
	std::ostream& operator<<(
		std::ostream& stream,
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		stream << automaton.states() << " states, " << std::endl
			<< automaton.transitions() << " transitions, " << std::endl
			<< automaton.epsilonTransitions() << " epsilon-transitions, and " << std::endl
			<< automaton.ambiguousTransitions() << " non-deterministic transitions."
			<< std::endl << std::endl;

		std::unordered_map<State_ConstIterator, integer,
			IteratorAddress_Hash> stateMap;

		integer stateId = 0;
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			stateMap[state] = stateId;
			++stateId;
		}

		stream << "Transitions:" << std::endl
			<< std::endl;

		for (auto transition = automaton.cTransitionBegin();
			transition != automaton.cTransitionEnd();
			++transition)
		{
			stream << "(" << stateMap[transition->from()] << ", ";
			if (transition->symbol().empty())
			{
				stream << "-";
			}
			else
			{
				stream << transition->symbol();
			};
			stream << ") --> " << stateMap[transition->to()] << std::endl;
		}

		stream << std::endl;

		stream << "Start states: { ";

		std::for_each(
			automaton.cStartBegin(),
			automaton.cStartEnd(),
			[&](const State_ConstIterator& state)
		{
			stream << stateMap[state] << " ";
		});

		stream << "}" << std::endl << std::endl;

		stream << "Final states: { ";

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			stream << stateMap[state] << " ";
		});

		stream << "}" << std::endl;

		return stream;
	}

}

#endif
