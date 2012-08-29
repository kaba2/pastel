// Description: Cloning pointer

#ifndef PASTELSYS_CLONE_PTR_H
#define PASTELSYS_CLONE_PTR_H

#include "pastel/sys/clone_ptr_concepts.h"
#include "pastel/sys/clone_ptr_private.h"

#include <cstddef>
#include <memory>
#include <utility>

#include <boost/operators.hpp>

namespace Pastel
{

	/*
	Note: The code here follows closely to that in the paper
	"A Preliminary Proposal for a Deep-Copying Smart Pointer",
	Walter Bright, 2012, WG21/N3339 = PL22.16/12-0029.
	*/

	template <
		typename Type_, 
		typename Cloner_ = typename ClonePtr_::Default_Cloner<Type_>::type, 
		typename Deleter_ = std::default_delete<Type_>>
	class ClonePtr
		: boost::less_than_comparable<ClonePtr<Type_, Cloner_, Deleter_>
		, boost::equality_comparable<ClonePtr<Type_, Cloner_, Deleter_>
		, boost::less_than_comparable2<ClonePtr<Type_, Cloner_, Deleter_>, std::nullptr_t
		, boost::equality_comparable2<ClonePtr<Type_, Cloner_, Deleter_>, std::nullptr_t
		> > > >
	{
	public:
		typedef Type_ Type;
		typedef Cloner_ Cloner;
		typedef Deleter_ Deleter;

	private:
		template<typename That>
		struct is_compatible
			: std::is_convertible<That*, Type*> 
		{
		};

	public:
		ClonePtr() 
			: data_(nullptr) 
		{ 
		}

		ClonePtr(std::nullptr_t) 
			: data_(nullptr) 
		{ 
		}

		template<typename That>
		explicit ClonePtr(That* that)
			: data_(that)
		{
			static_assert(
				is_compatible<That>::value,
				"ClonePtr's pointee type is incompatible.");

			static_assert(
				!std::is_polymorphic<Type>::value || 
				ClonePtr_::is_cloneable<Type>::value, 
				"ClonePtr's pointee type would slice on copying. "
				"Provide a clone() member function or remove polymorphism.");
		}

		ClonePtr(const ClonePtr& that )
			: data_(clone(that.data_))
		{ 
		}

		template<typename That>
		ClonePtr(const  ClonePtr<That, Cloner, Deleter>& that
			, typename std::enable_if<is_compatible<That>::value>::type * = nullptr)
			: data_(clone(that.data_))
		{ 
		}

		ClonePtr(ClonePtr&& that)
			: data_(that.release())
		{ 
		}

		template<typename That>
		ClonePtr(ClonePtr<That, Cloner, Deleter>&& that,
			typename std::enable_if< is_compatible<That>::value>::type * = nullptr)
			: data_(that.release())
		{ 
		}

		~ClonePtr()
		{ 
			reset(); 
		}

		void swap(ClonePtr& that) 
		{ 
			std::swap(data_, that.data_); 
		}

		ClonePtr& operator=(std::nullptr_t)
		{ 
			reset(nullptr); 
			return *this; 
		}

		ClonePtr& operator=(const ClonePtr& that)
		{ 
			ClonePtr copy(that); 
			swap(copy); 
			return *this; 
		}

		template<typename That >
		typename std::enable_if<is_compatible<That>::value, ClonePtr&>::type
			operator=(const ClonePtr<That, Cloner, Deleter>& that)
		{ 
			ClonePtr copy(that); 
			swap(copy); 
			return *this; 
		}

		ClonePtr& operator=(ClonePtr&& that)
		{ 
			ClonePtr copy(std::move(that)); 
			swap(copy);
			return *this; 
		}		

		template<typename That >
		typename std::enable_if<is_compatible<That>::value, ClonePtr&>::type
			operator=(ClonePtr<That, Cloner, Deleter>&& that)
		{ 
			ClonePtr copy(std::move(that)); 
			swap(copy); 
			return *this; 
		}

		bool operator<(const ClonePtr& that) const
		{
			return data_ < that.data_;
		}

		bool operator==(const ClonePtr& that) const
		{
			return data_ == that.data_;
		}

		bool operator<(std::nullptr_t) const
		{
			// data_ < nullptr;
			return false;
		}

		bool operator==(std::nullptr_t) const
		{
			return data_ == nullptr;
		}

		Type& operator*() const 
		{ 
			return *get(); 
		}

		Type* operator->() const 
		{ 
			return get(); 
		}

		Type* get() const 
		{ 
			return data_; 
		}

		Type* release() 
		{ 
			Type* old = data_; 
			data_ = nullptr; 
			return old; 
		}

		void reset(Type* that = nullptr) 
		{
			Deleter()(data_); 
			data_ = that;
		}

	private:
		template<typename That>
		Type* clone(const That* that) const
		{ 
			return that ? Cloner()(*that) : nullptr; 
		}

		Type* data_;
	}; 

	template<typename Type, typename Cloner, typename Deleter>
	void swap(
		ClonePtr<Type,Cloner,Deleter> &left, 
		ClonePtr<Type,Cloner,Deleter> & right)
	{ 
		left.swap(right); 
	}

}

#endif
