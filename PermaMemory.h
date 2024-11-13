/*
    PermaMemory is a helper library that well, helps you store memory permanently
    It provides various functions for offset assistance so you can write data and know where to find it next time
*/

#pragma once
#include <fstream>
#include <iostream>
#include "JMM.h"

static bool LogOffsets = true;

namespace JMM
{
    void SaveState(const std::string& Filename)
    {
        std::ofstream File(Filename, std::ios::binary);
        if (!File.is_open())
            return;

        File.write(Pool, POOL_SIZE);

        MemoryBlock* Current = FreeList;
        while (Current) 
        {
            size_t Offset = reinterpret_cast<char*>(Current) - Pool;
            File.write(reinterpret_cast<char*>(&Offset), sizeof(Offset));
            File.write(reinterpret_cast<char*>(&Current->Size), sizeof(Current->Size));
            File.write(reinterpret_cast<char*>(&Current->IsFree), sizeof(Current->IsFree));
            Current = Current->Next;
        }

        size_t EndMarker = static_cast<size_t>(-1);
        File.write(reinterpret_cast<char*>(&EndMarker), sizeof(EndMarker));

        File.close();
    }

    void LoadState(const std::string& Filename)
    {
        std::ifstream File(Filename, std::ios::binary);
        if (!File.is_open())
            return;

        File.read(Pool, POOL_SIZE);

        FreeList = nullptr;
        MemoryBlock** Current = &FreeList;
        size_t Offset;

        while (true) 
        {
            File.read(reinterpret_cast<char*>(&Offset), sizeof(Offset));
            if (Offset == static_cast<size_t>(-1)) break;

            *Current = reinterpret_cast<MemoryBlock*>(Pool + Offset);
            File.read(reinterpret_cast<char*>(&(*Current)->Size), sizeof((*Current)->Size));
            File.read(reinterpret_cast<char*>(&(*Current)->IsFree), sizeof((*Current)->IsFree));
            (*Current)->Next = nullptr;

            Current = &((*Current)->Next);
        }

        File.close();
    }

    template <typename T>
    T GetValueFromOffset(size_t Offset)
    {
        if (Offset + sizeof(T) > POOL_SIZE)
            return T();

        T* Location = reinterpret_cast<T*>(Pool + Offset);
        return *Location;
    }

    template <typename T>
    void SetValueAtOffset(size_t Offset, const T& Value)
    {
        if (Offset + sizeof(T) > POOL_SIZE)
            return;

        T* Location = reinterpret_cast<T*>(Pool + Offset);
        *Location = Value;
    }

    template <typename T>
    void* GetLocationOfOffset(size_t Offset)
    {
        if (Offset + sizeof(T) > POOL_SIZE)
            return nullptr;

        T* Location = reinterpret_cast<T*>(Pool + Offset);
        return Location;
    }

    size_t GetOffset(void* Location)
    {
        return reinterpret_cast<char*>(Location) - Pool;
    }

    void LogOffsetOf(void* Location)
    {
        size_t Offset = GetOffset(Location);
        std::cout << Offset << std::endl;
    }
}