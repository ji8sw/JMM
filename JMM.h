#pragma once

namespace JMM
{
	constexpr size_t POOL_SIZE = 1024 * 1024;
	char Pool[POOL_SIZE];

	struct MemoryBlock
	{
		size_t Size;
		bool IsFree;
		MemoryBlock* Next;
	};

	MemoryBlock* FreeList = reinterpret_cast<MemoryBlock*>(Pool);

	void InitializePool()
	{
		FreeList->Size = POOL_SIZE - sizeof(MemoryBlock);
		FreeList->IsFree = true;
		FreeList->Next = nullptr;
	}

	void* Allocate(size_t Bytes)
	{
		MemoryBlock* Current = FreeList;

		while (Current)
		{
			if (Current->IsFree && Current->Size >= Bytes)
			{
				if (Current->Size >= Bytes + sizeof(MemoryBlock))
				{
					MemoryBlock* NewBlock = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(Current) + sizeof(MemoryBlock) + Bytes);
					NewBlock->Size = Current->Size - Bytes - sizeof(MemoryBlock);
					NewBlock->IsFree = true;
					NewBlock->Next = Current->Next;

					Current->Size = Bytes;
					Current->Next = NewBlock;
				}

				Current->IsFree = false;
				return reinterpret_cast<char*>(Current) + sizeof(MemoryBlock);
			}
			Current = Current->Next;
		}
		return nullptr;
	}

	void Deallocate(void* Location)
	{
		if (!Location) return;

		MemoryBlock* Block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(Location) - sizeof(MemoryBlock));
		Block->IsFree = true;

		MemoryBlock* Current = FreeList;
		while (Current && Current->Next)
		{
			if (Current->IsFree && Current->Next->IsFree)
			{
				Current->Size += sizeof(MemoryBlock) + Current->Next->Size;
				Current->Next = Current->Next->Next;
			}
			else 
				Current = Current->Next;
		}
	}

	bool IsInBounds(void* Location)
	{
		return (Location && Location > Pool && Location < Pool + POOL_SIZE);
	}

	bool IsOccupied(void* Location)
	{
		if (!IsInBounds(Location))
			return false; // not within bounds

		MemoryBlock* Current = FreeList;
		while (Current) 
		{
			char* BlockStart = reinterpret_cast<char*>(Current) + sizeof(MemoryBlock);
			char* BlockEnd = BlockStart + Current->Size;

			if (Location >= BlockStart && Location < BlockEnd)
				return !Current->IsFree; // occupied

			Current = Current->Next;
		}

		return false; // couldnt find
	}
}