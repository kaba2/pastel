#ifndef PASTELSYS_BLOCKARRAY_H
#define PASTELSYS_BLOCKARRAY_H

#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

    template <typename Type, int Log2BlockSize = 8>
    class BlockArray
    {
    public:
        //! Constructs an empty array.
        /*!
        Time complexity: constant
        Exception safety: strong
        */
        BlockArray();

        //! Constructs an array with the given number of elements.
        /*!
        Time complexity: linear in 'elements'
        Exception safety: strong
        */
        explicit BlockArray(integer elements, const Type& data = Type());

        //! Constructs a copy of another array.
        /*!
        Time complexity: linear in elements of 'that'
        Exception safety: strong
        */
        BlockArray(const BlockArray& that);

        //! Destructs the array.
        /*!
        Time complexity: linear
        Exception safety: nothrow
        */
        ~BlockArray();

        //! Copies another array.
        /*!
        Time complexity: linear in elements of 'that'
        Exception safety: strong
        */
        BlockArray<Type, Log2BlockSize> operator=(const BlockArray& that);

        //! Swaps two arrays.
        /*!
        Time complexity: constant
        Exception safety: nothrow
        */
        void swap(BlockArray& that);

        //! Removes all elements.
        /*!
        Time complexity: linear
        Exception safety: nothrow
        */
        void clear();

        //! Returns the element at the given index.
        /*!
        Preconditions:
        index >= 0 && index < size()

        Time complexity: constant
        Exception safety: nothrow
        */
        const Type& operator[](integer index) const;

        //! Returns the element at the given index.
        /*!
        Preconditions:
        index >= 0 && index < size()

        Time complexity: constant
        Exception safety: nothrow
        */
        Type& operator[](integer index);

        //! Allocates enough space to hold 'size' elements.
        /*!
        Preconditions:
        size >= 0

        Time complexity: linear in the number of elements added
        Exception safety: strong

        No references are invalidated.
        */
        void resize(integer size, const Type& data = Type());

        //! Insert an element to the end of the array.
        /*!
        Time complexity: constant
        Exception safety: nothrow

        The time complexity is truly constant rather than
        amortized constant because new
        memory is allocated in constant size pieces.
        */
        void push_back(const Type& data);

        //! Removes the last element of the array.
        /*!
        Preconditions:
        size() >= 1

        Time complexity: constant
        Exception safety: nothrow
        */
        void pop_back();

        //! Returns the first element of the array.
        /*!
        Preconditions:
        !empty()

        Time complexity: constant
        Exception safety: nothrow
        */
        Type& front();

        //! Returns the first element of the array.
        /*!
        Preconditions:
        !empty()

        Time complexity: constant
        Exception safety: nothrow
        */
        const Type& front() const;

        //! Returns the last element of the array.
        /*!
        Preconditions:
        !empty()

        Time complexity: constant
        Exception safety: nothrow
        */
        Type& back();

        //! Returns the last element of the array.
        /*!
        Preconditions:
        !empty()

        Time complexity: constant
        Exception safety: nothrow
        */
        const Type& back() const;

        //! Returns the number of elements in the array.
        /*!
        Time complexity: constant
        Exception safety: nothrow
        */
        integer size() const;

        //! Returns the upper limit for resize() to work without reallocation.
        /*!
        Time complexity: constant
        Exception safety: nothrow
        */
        integer capacity() const;
        
    private:
        enum
        {
            BlockShift = Log2BlockSize,
            BlockSize = 1 << BlockShift,
            BlockMask = BlockSize - 1
        };

        // Adds a new block to the end of 'block_', initialized with 'data'.
        /*
        Time complexity: constant
        Exception safety: strong
        */
        void addBlock();

        /*
        Time complexity: linear in BlockSize
        Exception safety: nothrow
        */
        void removeBlock();

        /*
        Time complexity: linear in 'elements'
        Exception safety: nothrow
        */
        void constructElements(integer elements, const Type& data);

        /*
        Time complexity: linear in 'elements'
        Exception safety: strong
        */
        void constructElements(integer elements, const Type* sourceBlock);

        /*
        Time complexity: linear in 'elements'
        Exception safety: nothrow
        */
        void destructElements(integer elements);

        std::vector<Type*> block_;
        integer size_;
        integer lastBlockSize_;
    };

}

#include "pastel/sys/blockarray.hpp"

#endif
