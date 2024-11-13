# JMM
JMM (Ji9sw's Memory Manager) started out as string to make a memory allocator, then I decided to save the memory to a file,  then I decided we could make some stuff with that!

Now you can easily allocate memory of any type, and save it to a file, load it later, and go through it, thats basically what the Instructions library does.

The Instructions library has much room for extra instructions to make it more powerful, further, you could make a library that reads a script and converts it into actual instructions so you don't have to write programs in C++, and instead in our own instruction script instead! but that is yet to be made.

# JMM Instruction Book
JMM programs are made by placing an instruction followed by its parameters, if the interpreter cannot find the parameters required for the instruction, it fails, a good example of this is the `PRINT` instruction.

## Auto Instructions
Instructions can be easily created using their helper functions, for example `JMM::Print("Hello World!")` would allocate space for `JMM::Type` of type `PRINT` then allocate space for a `String`, and will automatically copy your content into the newly allocated string.

## Print
To first use the `PRINT` instruction, or any other instruction for that matter, you first allocate it memory, the type should be of `JMM::Type`, and you can set the instruction type when allocating the memory
after creating the instruction you should immediately allocate memory for the parameter, which in this case is a String.

You repeat this process for all instructions, while remembering not to allocate any parameters for instructions that don't take any.

## Break
The `BREAK` instruction is used to signal an end to the running program, it is always recommended to add one however if not the program will continue checking memory until it reaches the end of the Pool, this instruction takes no parameters.

## Clear
The `CLEAR` instruction is used to signal that the console should be cleared, this will delete all existing print statements giving you a fresh start, this instruction takes no parameters.

## Wait
The `WAIT` instruction pauses the application for a number of milliseconds, it takes the number of milliseconds as an `int`.

## Jump
The `JUMP` instruction increments the current offset by the provided offset, which is passed as a parameter of type `size_t`, this will allow you to jump forward or back.

