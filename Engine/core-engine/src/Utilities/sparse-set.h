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

    Iterator begin() {
        return Iterator(0,*this);
    }

    Iterator end() {
        return Iterator(size_,*this);
    }

    SparseSet();

    T& push_back()
    {
        COPIUM_ASSERT(size_ == N, "SPARSE SET IS ALREADY FULL");
        T& back = *reinterpret_cast<T*>(data + indexes[size_]);
        ++size_;
        back = T();
        return back;
    }

    T& push_back(const T& val)
    {
        COPIUM_ASSERT(size_ == N, "SPARSE SET IS ALREADY FULL");
        T& back = *reinterpret_cast<T*>(data+indexes[size_]);
        ++size_;
        back = val;
        return back;
    }

    T& push_back(std::initializer_list<T>&& rhsList)
    {
        COPIUM_ASSERT(size_ == N, "SPARSE SET IS ALREADY FULL");
        T& back = *reinterpret_cast<T*>(data + indexes[size_]);
        ++size_;
        back = T(rhsList);
        return back;
    }

    T& push_back(T&& val)
    {
        COPIUM_ASSERT(size_ == N, "SPARSE SET IS ALREADY FULL");
        PRINT("MOVE CONSTRUCTOR");
        T* pBack = reinterpret_cast<T*>(data + indexes[size_]) ;
        memcpy(pBack, &val, sizeof(T));
        ++size_;
        return *pBack;
    }

    template <typename... Args>
    T& emplace_back(Args&&... args)
    {
        T& back = *new (data + indexes[size_]) T(std::forward<Args>(args)...); // Construct the new element in the array
        ++size_;
        return back;
    }

    size_t AddFromDenseIndex(size_t);

    void Delete(size_t indexToDelete);

    void erase(T& val)
    {
        size_t denseIndex = &val - reinterpret_cast<T*>(data) >= size_;
        COPIUM_ASSERT(denseIndex, "Value is not an element of this array");
        //Find index first
        for (size_t i = 0; i < size_; ++i)
        {
            if (reinterpret_cast<T*>(data + indexes[i])  == &val)
            {
                size_t index = indexes[i];
                std::remove(indexes.begin(), indexes.begin()+size_, i);
                indexes[size_ - 1] = index;
                --size_;
                return;
            }
        }
        COPIUM_ASSERT(true, "Value is not an element of this array");
    }

    void erase(const Iterator& iter)
    {
        PRINT("BEFORE:");
        for (int i = 0; i < size_; ++i)
        {
            std::cout << indexes[i] << " ";
        }
        std::cout << std::endl;
        size_t index = indexes[iter.sparseIndex];
        std::remove(indexes.begin(), indexes.begin() + size_, iter.sparseIndex);
        indexes[size_-1] = index;
        PRINT("AFTER:");
        for (int i = 0; i < size_; ++i)
        {
            std::cout << indexes[i] << " ";
        }
        std::cout << std::endl;
        --size_;
    }

    size_t size() const
    {
        return size_;
    }

    T& operator[] (size_t i);

    size_t GetDenseIndex(size_t sparseIndex)
    {
        return indexes[sparseIndex];
    }

    T& DenseGet(size_t denseIndex)
    {
        return data[denseIndex];
    }

    bool DenseExists(size_t denseIndex)
    {
        for (size_t i : indexes)
        {
            if (i == denseIndex)
                return true;
        }
        return false;
    }

    size_t(&GetIndexes())[N]
    {
        return indexes;
    }

    void swap(size_t sparseIndex1, size_t sparseIndex2)
    {
        size_t tmp{ indexes[sparseIndex1] };
        indexes[sparseIndex1] = indexes[sparseIndex2];
        indexes[sparseIndex2] = indexes[sparseIndex1];
    }

    void swap(T& lhs, T& rhs)
    {
        size_t rhsDenseIndex = &rhs - reinterpret_cast<T*>(data) >= size_;
        size_t lhsDenseIndex = &lhs - reinterpret_cast<T*>(data) >= size_;
        COPIUM_ASSERT(rhsDenseIndex, "RHS is not an element of this array");
        COPIUM_ASSERT(lhsDenseIndex, "LHS is not an element of this array");
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
}

template <typename T, size_t N>
size_t SparseSet<T, N>::AddFromDenseIndex(size_t denseIndex)
{
    for (size_t i = 0; i < size_; ++i)
    {
        if (indexes[i] == denseIndex)
            return i;
    }
    ++size_;
    size_t* index{};
    for (size_t& i : indexes)
    {
        if (i == denseIndex)
        {
            index = &i;
            break;
        }
    }
    size_t tmp = indexes[size_ - 1];
    indexes[size_ - 1] = denseIndex;
    *index = tmp;
    data[indexes[size_ - 1]] = T();
    //Return sparse index, aka position of pooled object
    return size_ - 1;
}

template <typename T, size_t N>
void SparseSet<T, N>::Delete(size_t indexToDelete)
{
    for (size_t i = 0; i < size_; ++i)
    {
        size_t index = indexes[i];
        if (index == indexToDelete)
        {
            size_t tmp = index;
            indexes[i] = indexes[size_ - 1];
            indexes[size_ - 1] = tmp;
            --size_;
            return;
        }
    }
}

template <typename T, size_t N>
T& SparseSet<T, N>::operator[] (size_t i)
{
    COPIUM_ASSERT(i >= size_, "ARRAY OUT OF BOUNDS");
    return *reinterpret_cast<T*>(data + indexes[i]);
}

#endif // !SPARSE_SET_H