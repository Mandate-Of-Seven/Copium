#ifndef SPARSE_SET_H
#define SPARSE_SET_H

template <typename T, size_t N>
class SparseSet
{
public:
    SparseSet();

    size_t Add();

    size_t Add(const T& val);

    size_t AddFromDenseIndex(size_t);

    void Delete(size_t indexToDelete);

    void Delete(T* toDelete);

    size_t GetSize()
    {
        return size;
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

    size_t GetSparseIndex(size_t denseIndex)
    {
        for (size_t i = 0; i < size; ++i)
        {
            size_t index = indexes[i];
            if (index == denseIndex)
                return i;
        }
        return N;
    }

    void Swap(size_t sparseIndex1, size_t sparseIndex2)
    {
        size_t tmp{ indexes[sparseIndex1] };
        indexes[sparseIndex1] = indexes[sparseIndex2];
        indexes[sparseIndex2] = indexes[sparseIndex1];
    }

private:
    T data[N];
    size_t indexes[N];
    size_t size{ 0 };
};

template <typename T, size_t N>
SparseSet<T, N>::SparseSet()
{
    for (size_t i = 0; i < N; ++i)
    {
        indexes[i] = i;
    }
}

template <typename T, size_t N>
size_t SparseSet<T, N>::Add()
{
    ++size;
    return indexes[size - 1];
}

template <typename T, size_t N>
size_t SparseSet<T, N>::AddFromDenseIndex(size_t denseIndex)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (indexes[i] == denseIndex)
            return i;
    }
    ++size;
    size_t* index{};
    for (size_t& i : indexes)
    {
        if (i == denseIndex)
        {
            index = &i;
            break;
        }
    }
    size_t tmp = indexes[size - 1];
    indexes[size - 1] = denseIndex;
    *index = tmp;
    //Return sparse index, aka position of pooled object
    return size-1;
}

template <typename T, size_t N>
size_t SparseSet<T, N>::Add(const T& val)
{
    //ASSERT less than N
    data[indexes[size]] = val;
    ++size;
    return indexes[size - 1];
}

template <typename T, size_t N>
void SparseSet<T, N>::Delete(size_t indexToDelete)
{
    for (size_t i = 0; i < size; ++i)
    {
        size_t index = indexes[i];
        if (index == indexToDelete)
        {
            size_t tmp = index;
            indexes[i] = indexes[size - 1];
            indexes[size - 1] = tmp;
            --size;
            return;
        }
    }
}

template <typename T, size_t N>
void SparseSet<T, N>::Delete(T* toDelete)
{
    for (size_t i = 0; i < size; ++i)
    {
        size_t index = indexes[i];
        if (&data[index] == toDelete)
        {
            size_t tmp = index;
            indexes[i] = indexes[size - 1];
            indexes[size - 1] = tmp;
            --size;
            return;
        }
    }
}

template <typename T, size_t N>
T& SparseSet<T, N>::operator[] (size_t i)
{
    return data[indexes[i]];
}

#endif // !SPARSE_SET_H