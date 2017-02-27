#pragma once

#include "memory.h"
#include "error.h"
#include "random.h"

namespace Core
{

class Page
{
public:
    Page(Memory::PoolType poolType, Error& err);
    void* Map();
    void Unmap();
    void* MapAtomic();
    void UnmapAtomic(void* va);

    void* GetPagePtr();
    size_t GetSize();

    void Zero();
    Error FillRandom(Random& rng);
    int CompareContent(Page& other);

    virtual ~Page();

    size_t Read(void *buf, size_t len, size_t off);
    size_t Write(const void *buf, size_t len, size_t off);

private:
    Page(const Page& other) = delete;
    Page(Page&& other) = delete;
    Page& operator=(const Page& other) = delete;
    Page& operator=(Page&& other) = delete;

    Memory::PoolType PoolType;
    void* PagePtr;
};

class PageMap
{
public:
    PageMap(Page& page);
    virtual ~PageMap();
    void* GetAddress();
    void Unmap();
private:
    PageMap(PageMap&& other) = delete;
    PageMap(const PageMap& other) = delete;
    PageMap& operator=(const PageMap& other) = delete;
    PageMap& operator=(PageMap&& other) = delete;

    Page& PageRef;
    void* Address;
};

}