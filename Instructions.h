/*
    Instructions is a library that helps you build programs based on instructions
    It provides a standard set of instruction types, along with helper functions that properly allocate parameters for those instructions
    Please visit the JMM Instruction Book for help
*/

#pragma once
#include <string> // FOR PRINT
#include <thread> // FOR WAIT
#define MSTRLEN 128
using String = char[MSTRLEN];

size_t CurrentOffset = 24;
bool WantsToEnd = false;

namespace JMM
{
    enum Type
    {
        PRINT,
        BREAK,
        CLEAR,
        WAIT,
        JUMP
    };

    enum InstructionSizes
    {
        PRINTSZ = sizeof(String) + 28,
        BREAKSZ = 28,
        CLEARSZ = 28,
        WAITSZ = sizeof(int) + 28,
        JUMPSZ = sizeof(size_t) + 28
    };

    void Print(String Content)
    {
        JMM::Type* NewPrintInstruction = JMM::Allocate<JMM::Type>();
        *NewPrintInstruction = Type::PRINT;

        std::strncpy(*JMM::Allocate<String>(), Content, MSTRLEN - 1);
    }

    void Break()
    {
        JMM::Type* BreakInstruction = JMM::Allocate<JMM::Type>();
        *BreakInstruction = Type::BREAK;
    }

    void Clear()
    {
        JMM::Type* ClearInstruction = JMM::Allocate<JMM::Type>();
        *ClearInstruction = Type::CLEAR;
    }

    void Wait(int Milliseconds)
    {
        JMM::Type* WaitInstruction = JMM::Allocate<JMM::Type>();
        *WaitInstruction = Type::WAIT;
        *JMM::Allocate<int>() = Milliseconds;
    }

    void JumpInstruction(size_t OffsetToAdd)
    {
        JMM::Type* JumpInstruction = JMM::Allocate<JMM::Type>();
        *JumpInstruction = Type::JUMP;
        *JMM::Allocate<size_t>() = OffsetToAdd;
    }

    template <typename T>
    void Jump()
    {
        CurrentOffset += sizeof(T) + 24;
    }

    void ExecuteInstruction(JMM::Type Instruction)
    {
        switch (Instruction)
        {
        case JMM::Type::PRINT:
            std::cout << JMM::GetValueFromOffset<std::string>(CurrentOffset + 28).c_str() << std::endl;
            Jump<String>(); // add the string position to the current offset
            break;
        case JMM::Type::BREAK:
            WantsToEnd = true;
            break;
        case JMM::Type::CLEAR:
            system("cls");
            break;
        case JMM::Type::JUMP:
            CurrentOffset += JMM::GetValueFromOffset<size_t>(CurrentOffset + 28) + 24;
            break;
        case JMM::Type::WAIT:
            std::this_thread::sleep_for(std::chrono::milliseconds(JMM::GetValueFromOffset<int>(CurrentOffset + 28)));
            Jump<int>();
            break;
        }
    }
}