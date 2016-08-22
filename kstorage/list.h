#pragma once

#include "main.h"
#include "list_entry.h"

template <class T> class LinkedList
{
public:

    class Iterator
    {
    public:
        Iterator(LinkedList& List)
        {
            CurrListEntry = List.ListHead.Flink;
            EndList = &List.ListHead;
        }
        T& Get()
        {
            KBUG_ON(CurrListEntry == EndList);
            LinkedListNode* node = CONTAINING_RECORD(CurrListEntry,
                                                     LinkedListNode,
                                                     ListLink);
            return node->Value;
        }

        bool IsValid()
        {
            return (CurrListEntry != EndList) ? true : false;
        }

        void Next()
        {
            if (IsValid())
            {
                CurrListEntry = CurrListEntry->Flink;
            }
        }

        void Erase()
        {
            KBUG_ON(!IsValid());

            ListEntry* next = CurrListEntry->Flink;

            LinkedListNode* node = CONTAINING_RECORD(CurrListEntry,
                                                     LinkedListNode,
                                                     ListLink);
            RemoveEntryList(&node->ListLink);
            delete node;
            CurrListEntry = next;
        }

        virtual ~Iterator()
        {

        }
    private:
        Iterator() = delete;
        Iterator(const Iterator& other) = delete;
        Iterator& operator=(const Iterator& other) = delete;
        Iterator& operator=(Iterator&& other) = delete;
        ListEntry* CurrListEntry;
        ListEntry* EndList;
    };

    LinkedList()
        : MemoryType(MemType::Kernel)
    {
        InitializeListHead(&ListHead);
    }

    LinkedList(MemType memType)
        : MemoryType(memType)
    {
        InitializeListHead(&ListHead);
    }

    bool AddHead(const T& value)
    {
        LinkedListNode* node = new (MemoryType) LinkedListNode(value);

        if (!node)
        {
            return false;
        }
        InsertHeadList(&ListHead, &node->ListLink);
        return true;
    }

    bool AddTail(const T& value)
    {
        LinkedListNode* node = new (MemoryType) LinkedListNode(value);

        if (!node)
        {
            return false;
        }
        InsertTailList(&ListHead, &node->ListLink);
        return true;
    }

    bool AddTail(T&& value)
    {
        LinkedListNode* node = new (MemoryType)
                                LinkedListNode(util::move(value));
        if (!node)
        {
            return false;
        }
        InsertTailList(&ListHead, &node->ListLink);
        return true;
    }

    T& Head()
    {
        LinkedListNode* node;

        KBUG_ON(IsListEmpty(&ListHead));
        node = CONTAINING_RECORD(ListHead.Flink, LinkedListNode, ListLink);
        return node->Value;
    }

    T& Tail()
    {
        LinkedListNode* node;

        KBUG_ON(IsListEmpty(&ListHead));
        node = CONTAINING_RECORD(ListHead.Blink, LinkedListNode, ListLink);
        return node->Value;
    }

    void PopHead()
    {
        LinkedListNode* node;

        KBUG_ON(IsListEmpty(&ListHead));
        node = CONTAINING_RECORD(RemoveHeadList(&ListHead),
                                 LinkedListNode, ListLink);
        delete node;
    }

    void PopTail()
    {
        LinkedListNode* node;

        KBUG_ON(IsListEmpty(&ListHead));
        node = CONTAINING_RECORD(RemoveTailList(&ListHead),
                                 LinkedListNode, ListLink);
        delete node;
    }

    bool IsEmpty()
    {
        return (IsListEmpty(&ListHead)) ? true : false;
    }

    virtual ~LinkedList()
    {
        Release();
    }

    LinkedList(LinkedList&& other)
    {
        if (!IsListEmpty(&other.ListHead))
            ListHead = other.ListHead;
        else
            InitializeListHead(&ListHead);

        MemoryType = other.MemoryType;
        InitializeListHead(&other.ListHead);
    }

    LinkedList& operator=(LinkedList&& other)
    {
        Release();

        if (!IsListEmpty(&other.ListHead))
            ListHead = other.ListHead;
        else
            InitializeListHead(&ListHead);

        MemoryType = other.MemoryType;
        InitializeListHead(&other.ListHead);
        return *this;
    }

private:
    LinkedList(const LinkedList& other) = delete;
    LinkedList& operator=(const LinkedList& other) = delete;

    void Release()
    {
        LinkedListNode* node;
        while (!IsListEmpty(&ListHead))
        {
            node = CONTAINING_RECORD(RemoveHeadList(&ListHead),
                                     LinkedListNode, ListLink);
            delete node;
        }
    }
    class LinkedListNode
    {
    public:
        LinkedListNode(const T& value)
        {
            InitializeListHead(&ListLink);
            Value = value;
        }
        LinkedListNode(T&& value)
        {
            InitializeListHead(&ListLink);
            Value = util::move(value);
        }
        virtual ~LinkedListNode()
        {
        }
        ListEntry ListLink;
        T Value;
    private:
        LinkedListNode() = delete;
        LinkedListNode(const LinkedListNode& other) = delete;
        LinkedListNode& operator=(const LinkedListNode& other) = delete;
        LinkedListNode& operator=(LinkedListNode&& other) = delete;
    };
    ListEntry ListHead;
    MemType MemoryType;
};