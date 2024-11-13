#pragma once
#include "JMM.h"

namespace JMM
{
	template <typename Type> Type* Allocate()
	{
		return (Type*)Allocate(sizeof(Type));
	}

	template <typename Type> Type* Copy(Type* Existing)
	{
		Type* Allocated = (Type*)Allocate(sizeof(Type));
		*Allocated = *Existing;
		return Allocated;
	}
}