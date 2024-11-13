#include "JMM.h"
#include "AllocatorHelper.h"
#include "PermaMemory.h"
#include "Instructions.h"

std::string PermaLocation = "Memory.perma";

int main()
{
    JMM::InitializePool();
    JMM::Print("New Program");
    JMM::Wait(5000);
    JMM::Clear();
    JMM::Print("Begin");

    JMM::SaveState(PermaLocation);
    return 0;
}