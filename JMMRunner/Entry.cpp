#include "JMM.h"
#include "AllocatorHelper.h"
#include "PermaMemory.h"
#include "Instructions.h"

std::string PermaLocation = "Memory.perma";

int main()
{
    JMM::InitializePool();
    JMM::LoadState(PermaLocation);

    while (true)
    {
        if (WantsToEnd) break;
        void* NextInstruction = JMM::GetLocationOfOffset<JMM::Type>(CurrentOffset);
        if (JMM::IsInBounds(NextInstruction) && JMM::IsOccupied(NextInstruction))
        {
            JMM::ExecuteInstruction(JMM::GetValueFromOffset<JMM::Type>(CurrentOffset));
        }
        else if (!JMM::IsInBounds(NextInstruction)) WantsToEnd = true; // we probably reached the end and there was no BREAK instruction
        CurrentOffset += 28;
    }

    return 0;
}