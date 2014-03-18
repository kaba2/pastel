// Description: Iterator macros
// Documentation: iterators.txt

#ifndef PASTELSYS_ITERATOR_MACROS_H
#define PASTELSYS_ITERATOR_MACROS_H

#define PASTEL_ITERATOR_FUNCTIONS_PREFIX(prefix, name, assign) \
	prefix##Iterator name() \
	{ \
		return prefix##Iterator(assign); \
	} \
	\
	PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(prefix, name, assign)

#define PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(prefix, name, assign) \
	prefix##ConstIterator name() const \
	{ \
		return prefix##ConstIterator(assign); \
	} \
	\
	prefix##ConstIterator c##name() const \
	{ \
		return prefix##ConstIterator(assign); \
	}

#define PASTEL_ITERATOR_FUNCTIONS(name, assign) \
	PASTEL_ITERATOR_FUNCTIONS_PREFIX(, name, assign)

#define PASTEL_CONST_ITERATOR_FUNCTIONS(name, assign) \
	PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(, name, assign)

#define PASTEL_RANGE_FUNCTIONS_PREFIX(prefix, name, begin_, end_) \
	prefix##Range name() \
	{ \
		return Pastel::range( begin_ (), end_ ()); \
	} \
	\
	PASTEL_CONST_RANGE_FUNCTIONS_PREFIX(prefix, name, begin_, end_)

#define PASTEL_CONST_RANGE_FUNCTIONS_PREFIX(prefix, name, begin_, end_) \
	prefix##ConstRange c##name() const \
	{ \
		return Pastel::range( c##begin_ (), c##end_ ()); \
	}

#define PASTEL_RANGE_FUNCTIONS(name, begin, end) \
	PASTEL_RANGE_FUNCTIONS_PREFIX(, name, begin, end)

#define PASTEL_CONST_RANGE_FUNCTIONS(name, begin, end) \
	PASTEL_CONST_RANGE_FUNCTIONS_PREFIX(, name, begin, end)

#define PASTEL_RANGE_ALGORITHM(FunctionName, Name) \
	class Name \
	{ \
	public: \
		template <typename Range> \
		void operator()(const Range& range) const \
		{ \
			FunctionName(range); \
		} \
	};

#endif