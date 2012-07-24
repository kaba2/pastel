#ifndef PASTEL_AUTOMATON_HPP
#define PASTEL_AUTOMATON_HPP

#include "pastel/sys/automaton.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol>::Automaton()
		: graph_()
		, transitionSet_()
		, rejectGraph_()
		, startState_(graph_.vertexEnd())
		, rejectState_(rejectGraph_.vertexEnd())
	{
		construct();
	}

	template <typename State, typename Symbol>
	Automaton<State, Symbol>::Automaton(Automaton&& that)
		: graph_()
		, transitionSet_()
		, rejectGraph_()
		, startState_(graph_.end())
		, rejectState_(rejectGraph_.vertexEnd())
	{
		construct();
		swap(that);
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::operator=(Automaton that)
		-> Automaton&
	{
		swap(that);
		return *this;
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::clear()
	{
		// The rejectGraph and rejectState
		// are kept the same.

		graph_.clear();
		transitionSet_.clear();
		startState_ = rejectState_;
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::clearTransitions()
	{
		transitionSet_.clear();
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::swap(Automaton& that)
	{
		using std::swap;

		graph_.swap(that.graph_);
		transitionSet_.swap(that.transitionSet_);
		rejectGraph_.swap(that.rejectGraph_);
		swap(startState_, that.startState_);
		swap(rejectState_, that.rejectState_);
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::addState(State state) 
		-> State_Iterator
	{
		auto iter = graph_.addVertex(std::move(state));
		return iter;
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::removeState(
		State_ConstIterator state) 
	{
		Vertex_ConstIterator vertex =
			state.base();

		// Remove all transitions that are 
		// incoming or outgoing at this vertex.
		while(vertex->incidentEdges() > 0)
		{
			removeTransition(
				vertex->cbegin()->edge());
		}

		// Remove the vertex.
		graph_.removeVertex(state);
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cast(
		State_ConstIterator state)
		-> State_Iterator
	{
		return graph_.cast(state.base());
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::setStartState(
		State_ConstIterator state)
	{
		startState_ = cast(state);
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::startState() const 
		-> State_ConstIterator
	{
		return startState_;
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::rejectState() const 
		-> State_ConstIterator
	{
		return rejectState_;
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::stateBegin() 
		-> State_Iterator
	{
		return graph_.vertexBegin();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cStateBegin() const
		-> State_ConstIterator
	{
		return graph_.cVertexBegin();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::stateEnd() 
		-> State_Iterator
	{
		return graph_.vertexEnd();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cStateEnd() const
		-> State_ConstIterator
	{
		return graph_.cVertexEnd();
	}

	template <typename State, typename Symbol>
	integer Automaton<State, Symbol>::states() const
	{
		return graph_.vertices();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::addTransition(
		State_ConstIterator fromState,
		Symbol symbol,
		State_ConstIterator toState)
		-> Transition_ConstIterator
	{
		auto transition = graph_.addEdge(
			fromState.base(), toState.base(), symbol);

		transitionSet_.emplace(
			std::make_pair(Transition(fromState, symbol), 
			transition));

		return transition;
	}

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::removeTransition(
		const Transition_ConstIterator& transition)
	{
		transitionSet_.erase(
			Transition(transition->from(), transition->data()));
		graph_.erase(transition);
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::findTransition(
		const Transition& transition) const
		-> Transition_ConstIterator
	{
		auto iter = transitionSet_.find(transition);
		return iter->second;
	}

	template <typename State, typename Symbol>
	bool Automaton<State, Symbol>::existsTransition(
		const Transition& transition) const
	{
		return findTransition(transition) != cTransitionEnd();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cTransitionBegin() const
		-> Transition_ConstIterator
	{
		return graph_.cEdgeBegin();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cTransitionEnd() const
		-> Transition_ConstIterator
	{
		return graph_.cEdgeEnd();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cTransitionBegin(
		const State_ConstIterator& state) const
		-> Transition_ConstIterator
	{
		return state.base()->cbegin();
	}

	template <typename State, typename Symbol>
	auto Automaton<State, Symbol>::cTransitionEnd(
		const State_ConstIterator& state) const
		-> Transition_ConstIterator
	{
		return state.base()->cend();
	}

	template <typename State, typename Symbol>
	integer Automaton<State, Symbol>::transitions() const
	{
		return graph_.edges();
	}

	// Private

	template <typename State, typename Symbol>
	void Automaton<State, Symbol>::construct()
	{
		// Construct the reject state.
		rejectState_ = rejectGraph_.addVertex();

		// The start-state is the reject-state
		// in the beginning.
		startState_ = rejectState_;
	}

}

namespace Pastel
{

	template <typename State, typename Symbol>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol symbol)
		-> typename Automaton<State, Symbol>::State_ConstIterator
	{
		auto transition = automaton.findTransition(
			Automaton<State, Symbol>::Transition(state, symbol));
		if (transition != automaton.cTransitionEnd())
		{
			// There is such a transition. 
			// Return the target state.
			return transition->to();
		}
		
		// There is no such transition. 
		// Return the reject state.
		return automaton.rejectState();
	}

	template <typename State, typename Symbol,
		typename Symbol_ConstRange>
	auto transition(
		const Automaton<State, Symbol>& automaton,
		typename Automaton<State, Symbol>::State_ConstIterator state,
		Symbol_ConstRange symbolSet)
		-> typename Automaton<State, Symbol>::State_ConstIterator
	{
		for (auto symbol = symbolSet.begin();
			symbol != symbolSet.end();
			++symbol)
		{
			state = transition(automaton, state, *symbol);
			if (state == automaton.rejectState())
			{
				// There was no such transition;
				// we are now in the reject state. Since
				// there are no transitions away from
				// the reject state, we can stop now.
				break;
			}
		}

		return state;
	}

}

#endif
