#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include <Debugging/logging-system.h>
#include <initializer_list>
#include <array>

template <typename T, size_t N>
class SparseSet
{
    //Uninitialized memory
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
    std::array<size_t,N>indexes;
    size_t size_{ 0 };
public:
    class Iterator
    {
        SparseSet<T, N>& arr;
        size_t sparseIndex;
        friend class SparseSet;
    public:
        Iterator(size_t _sparseIndex, SparseSet<T, N>& _arr) : sparseIndex(_sparseIndex), arr{ _arr } {}

        T& operator*() const
        {
            return arr[sparseIndex];
        }

        Iterator operator++() {
            ++sparseIndex;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return sparseIndex == other.sparseIndex;
        }

        bool operator!=(const Iterator& other) const {
            return sparseIndex != other.sparseIndex;
        }
    };

    class ReverseIterator
    {
        SparseSet<T, N>& arr;
        int sparseIndex;
        friend class SparseSet;
    public:
        ReverseIterator(size_t _sparseIndex, SparseSet<T, N>& _arr) : sparseIndex(_sparseIndex), arr{ _arr } {}

        T& operator*() const
        {
            return arr[sparseIndex];
        }

        ReverseIterator operator++() {
            --sparseIndex;
            return *this;
        }

        ReverseIterator operator++(int) {
            ReverseIterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator==(const ReverseIterator& other) const {
            return sparseIndex == other.sparseIndex;
        }

        bool operator!=(const ReverseIterator& other) const {
            return sparseIndex != other.sparseIndex;
        }
    };

    Iterator begin() {
        return Iterator(0,*this);
    }

    Iterator end() {
        return Iterator(size_,*this);
    }

    ReverseIterator rbegin() {
        return ReverseIterator(size_ - 1, *this);
    }

    ReverseIterator rend() {
        return ReverseIterator(-1, *this);
    }

    SparseSet();


    ~SparseSet()
    {
        for (T& element: *this)
        {
            element.~T();
        }
        PRINT("SPARSE SET DECONSTRUCTOR ");
    }

    template <typename... Args>
    T& emplace_back(Args&&... args)
    {
        T& back = *new (data + indexes[size_]) T(std::forward<Args>(args)...); // Construct the new element in the array
        ++size_;
        return back;
    }

    void erase(T& val)
    {
        //Find index first
        for (size_t i = 0; i < size_; ++i)
        {
            if (reinterpret_cast<T*>(data + indexes[i])  == &val)
            {
                size_t index = indexes[i];

                reinterpret_cast<T*>(data)[index].~T();

                std::remove(indexes.begin(), indexes.begin()+size_, index);
                indexes[size_ - 1] = index;
                --size_;
                return;
            }
        }
        COPIUM_ASSERT(true, "Value is not an element of this array");
    }

    void erase(const Iterator& iter)
    {
        COPIUM_ASSERT(size_ == 0, "Can't erase from empty array");
        size_t index = indexes[iter.sparseIndex];
        reinterpret_cast<T*>(data)[index].~T();
        PRINT("BEFORE:");
        for (size_t j = 0; j < size_; ++j)
        {
            std::cout << indexes[j] << " ";
        }
        std::remove(indexes.begin(), indexes.begin() + size_, index);
        indexes[size_ - 1] = index;
        PRINT("AFTER:");
        for (size_t j = 0; j < size_; ++j)
        {
            std::cout << indexes[j] << " ";
        }
        PRINT("");
        --size_;
    }

    size_t size() const
    {
        return size_;
    }

    T& operator[] (size_t i);

    bool exists(T* pValue)
    {
        size_t denseIndex = pValue - static_cast<T*>(data);
        for (size_t i = 0; i < size_; ++i)
        {
            if (indexes[i] == denseIndex)
            {
                return true;
            }
        }
        return false;
    }

    void swap(size_t sparseIndex1, size_t sparseIndex2)
    {
        size_t tmp{ indexes[sparseIndex1] };
        indexes[sparseIndex1] = indexes[sparseIndex2];
        indexes[sparseIndex2] = tmp;
    }

    void swap(T& lhs, T& rhs)
    {
        size_t rhsDenseIndex = &rhs - reinterpret_cast<T*>(data) ;
        size_t lhsDenseIndex = &lhs - reinterpret_cast<T*>(data) ;
        COPIUM_ASSERT(rhsDenseIndex >= size_, "RHS is not an element of this array");
        COPIUM_ASSERT(lhsDenseIndex >= size_, "LHS is not an element of this array");
        if (lhsDenseIndex > rhsDenseIndex)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                size_t& index = indexes[i];
                if (index == rhsDenseIndex)
                    index = lhsDenseIndex;
                else if (index == lhsDenseIndex)
                {
                    index = rhsDenseIndex;
                    return;
                }
            }
        }
        else if (lhsDenseIndex < rhsDenseIndex)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                size_t& index = indexes[i];
                if (index == lhsDenseIndex)
                    index = rhsDenseIndex;
                else if (index == rhsDenseIndex)
                {
                    index = lhsDenseIndex;
                    return;
                }
            }
        }
    }

    void clear(){ size_ = 0;}

    bool empty() const { return !size_; }

    template <typename T, size_t N>
    friend std::ostream& operator<<(std::ostream& stream, SparseSet<T, N>& sS);
};

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, SparseSet<T, N>& sS)
{
    for (size_t i{ 0 }; i < sS.size_; ++i)
    {
        stream << sS.indexes[i] << ",";
    }
    return stream;
}

template <typename T, size_t N>
SparseSet<T, N>::SparseSet()
{
    for (size_t i = 0; i < N; ++i)
    {
        indexes[i] = i;
    }
    PRINT("SPARSE SET CONSTRUCTED");
}

template <typename T, size_t N>
T& SparseSet<T, N>::operator[] (size_t i)
{
    COPIUM_ASSERT(i >= size_, "ARRAY OUT OF BOUNDS");
    return *reinterpret_cast<T*>(data + indexes[i]);
}

#endif // !SPARSE_SET_H