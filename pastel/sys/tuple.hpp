#ifndef PASTELSYS_TUPLE_HPP
#define PASTELSYS_TUPLE_HPP

#include "pastel/sys/tuple.h"

#include <vector>

namespace Pastel
{

    template <typename Type>
    class Tuple<Unbounded, Type>
    {
    private:
        enum
        {
            N = Unbounded
        };

    public:
        // Using default constructor.
        // Using default copy constructor.
        // Using default assignment.
        // Using default destructor.

        typedef Type value_type;
        typedef Type* iterator;
        typedef const Type* const_iterator;
        typedef Type* reverse_iterator;
        typedef const Type* const_reverse_iterator;
        typedef Type* pointer;
        typedef const Type* const_pointer;
        typedef Type& reference;
        typedef const Type& const_reference;
        typedef integer difference_type;
        typedef integer size_type;

        Tuple()
            : data_()
        {
        }

        explicit Tuple(const Type& that)
            : data_()
        {
            set(that);
        }

        ~Tuple()
        {
        }

        void swap(Tuple& that)
        {
            data_.swap(that.data_);
        }

        void setSize(integer size)
        {
            ENSURE1(size >= 0, size);

            resize(size);
        }

        void resize(integer size)
        {
            ENSURE1(size >= 0, size);

            data_.resize(size);
        }

        void set(const Type& that)
        {
            const integer n = size();

            for (integer i = 0;i < n;++i)
            {
                data_[i] = that;
            }
        }

        Type& front()
        {
            return data_[0];
        }

        const Type& front() const
        {
            return data_[0];
        }

        Type& back()
        {
            return data_[N - 1];
        }

        const Type& back() const
        {
            return data_[N - 1];
        }

        Type* begin()
        {
            return &data_[0];
        }

        const Type* begin() const
        {
            return &data_[0];
        }

        Type* end()
        {
            return &data_[0] + N;
        }

        const Type* end() const
        {
            return &data_[0] + N;
        }

        size_type capacity() const
        {
            return data_.capacity();
        }

        size_type size() const
        {
            return data_.size();
        }

        size_type max_size() const
        {
            return data_.max_size();
        }

        bool empty() const
        {
            return data_.empty();
        }

        Type& at(integer index)
        {
            PENSURE2(index >= 0 && index < N, index, N);

            return data_[index];
        }

        const Type& at(integer index) const
        {
            PENSURE2(index >= 0 && index < N, index, N);

            return data_[index];
        }

        Type& operator[](integer index)
        {
            PENSURE2(index >= 0 && index < N, index, N);
            return data_[index];
        }

        const Type& operator[](integer index) const
        {
            PENSURE2(index >= 0 && index < N, index, N);

            return data_[index];
        }

    private:
        std::vector<Type> data_;
    };

}

#endif
