#pragma once

#include "memory.h"
#include "error.h"
#include "bug.h"
#include "new.h"
#include "random.h"

namespace Core
{

template<class T, Memory::PoolType PoolType = Memory::PoolType::Kernel>
class Vector
{
public:
    Vector()
        : Arr(nullptr), Size(0), Capacity(0)
    {
    }

    size_t GetSize() const
    {
        return Size;
    }

    size_t GetCapacity() const
    {
        return Capacity;
    }

    T& operator[](size_t index)
    {
        panic(index < 0 || index >= Size);
        return Arr[index];
    }

    bool Reserve(size_t capacity)
    {
        if (capacity <= Capacity)
            return true;

        T* newArr = new (PoolType) T[capacity];
        if (!newArr)
            return false;

        if (Arr)
        {
            for (size_t i = 0; i < Size; i++)
            {
                newArr[i] = Memory::Move(Arr[i]);
            }
            delete[] Arr;
        }
        Arr = newArr;
        Capacity = capacity;
        return true;
    }

    bool ReserveAndUse(size_t capacity)
    {
        if (!Reserve(capacity))
            return false;
        Size = Capacity;
        return true;
    }

    bool Truncate(size_t size)
    {
        if (size > Size)
            return false;

        Size = size;
        return true;
    }

    bool PushBack(T&& e)
    {
        if (Size == Capacity)
        {
            if (!Reserve(2*Size + 1))
                return false;
        }
        Arr[Size++] = Memory::Move(e);
        return true;
    }

    bool PushBack(const T& e)
    {
        if (Size == Capacity)
        {
            if (!Reserve(2*Size + 1))
                return false;
        }
        Arr[Size++] = e;
        return true;
    }

    const T* GetConstBuf() const
    {
        return Arr;
    }

    T* GetBuf()
    {
        return Arr;
    }

    virtual ~Vector()
    {
        Release();
    }

    void Clear()
    {
        Release();
    }

    Vector(Vector&& other)
    {
        Arr = other.Arr;
        Size = other.Size;
        Capacity = other.Capacity;
        other.Arr = nullptr;
        other.Size = 0;
        other.Capacity = 0;
    }

    Vector& operator=(Vector&& other)
    {
        Release();
        Arr = other.Arr;
        Size = other.Size;
        Capacity = other.Capacity;
        other.Arr = nullptr;
        other.Size = 0;
        other.Capacity = 0;
        return *this;
    }

    Vector(const Vector& other, Error err)
    {
        if (!err.Ok())
            return;

        T* Arr = new (PoolType) T[other.Capacity];
        if (!Arr)
        {
            err = MakeError(Error::NoMemory);
            return;
        }

        for (size_t i = 0; i < other.Size; i++)
        {
            Arr[i] = other.Arr[i];
        }
        Size = other.Size;
        Capacity = other.Capacity;
    }

    void Shuffle()
    {
        if (Size < 2)
            return;

        for (size_t i = (Size - 1); i >= 1; i--)
        {
            size_t j = Random::GetSizeT(i + 1);
            Memory::Swap(Arr[i], Arr[j]);
        }
    }

private:
    Vector(const Vector& other) = delete;
    Vector& operator=(const Vector& other) = delete;

    void Release()
    {
        if (Arr)
        {
            delete[] Arr;
            Arr = nullptr;
        }
        Size = 0;
        Capacity = 0;
    }

    T* Arr;
    size_t Size;
    size_t Capacity;
};

}