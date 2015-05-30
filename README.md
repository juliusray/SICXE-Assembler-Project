# SICXE-Assembler-Project

This is a C++ semester-long team project that is able to parse tokens of assembly language opcodes for the SICXE computer architecture, its operands, and user-created symbols to create a listing file that contains addresses for each opcode and label. The listing file also lists the machine code produced by each opcode. 

### Team Members:

Julius Inocencio
Travis Barre
Ehsan Eshragh
Michael Reese


# How to run:

*Note: Must be using a UNIX environment in order to run the "make" command

1. Clone the repo and bring command line prompt to the directory with the source files
2. Type the command "make" to initiate the Makefile
3. An executable file called "sicxe_asm" will be created


# Usage:

params: assembly source files (in either .asm or .txt)

Sample files are provided for test purposes

On the command line type the executable filename followed by the assembly source file

	- sicxe_asm source1.asm

# Classes

## file_parser
This class is responsible for parsing assembly source code into separate tokens: labels, opcodes, operands, and comments. 


## opcodetab
This class stores information about operation codes used in the SICXE architecture. This includes: the opcode itself, the machine code it produces, and the number of bytes needed to encode the instruction. The data is stored in a STL map where the opcode name is the key and the value is a pair (machine code and size of instruction).

	- map < opcode_name < machine_code, code_size > > opcodemap 


## symtab
This class stores user-defined symbols into a STL map container. The name of the symbol acts as the key, while the address of that symbol represents the value. 

	- map < symbol_name, address > symbol_map


##sicxe_asm
This class prepares the listing file that is generated after parsing the assembly source code. It uses the file_parser to tokenize each field, the opcodetab to determine the machine code produced and how to handle its operand, and the symbtab to keep track of user-defined symbols. 

This is a two-pass assembler. The first pass stores addresses, labels, opcodes, and operands. The second pass calculates the machine code for each instruction.
