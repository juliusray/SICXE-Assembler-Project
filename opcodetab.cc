/*	Travis Barre, Julius Inocencio, Ehsan Eshragh, Michael Reese
	Team Oregon
	masc1187
	prog4
	opcodetab.cc
	CS530, Spring 2014
*/

#include "opcodetab.h"
#include <iostream>

#define NUMBER_OF_OPCODES 101

using namespace std;

// ctor
// creates a new dictionary structure and loads all of the opcodes for 
// the SIC/XE architecture into the table.  Use the STL map for this.
opcodetab::opcodetab() {
	for(unsigned int i = 0; i < NUMBER_OF_OPCODES; i++) 
		add_opcode(opcodes[i], machinecodes[i], sizes[i]);
}

// takes a SIC/XE opcode and returns the machine code 
// equivalent as a two byte string in hexadecimal.
// Example:  get_machine_code("ADD") returns the value 18
// Note that opcodes may be prepended with a '+'.
// throws an opcode_error_exception if the opcode is not 
// found in the table.
string opcodetab::get_machine_code(string s) {
	string tmp = to_uppercase(s);
	if(opcode_exists(tmp)) {
		pair <string, int> temppair = opcodemap[tmp];
		return temppair.first;
	}

	throw opcode_error_exception("- Invalid opcode: " + s);
}

// takes a SIC/XE opcode and returns the number of bytes 
// needed to encode the instruction, which is an int in
// the range 1..4.
// NOTE: the opcode must be prepended with a '+' for format 4.
// throws an opcode_error_exception if the opcode is not 
// found in the table.        
int opcodetab::get_instruction_size(string s) {
	string tmp = to_uppercase(s);
	if(opcode_exists(tmp)) {
		pair <string, int> temppair = opcodemap[tmp];
		return temppair.second;
	}

	throw opcode_error_exception("- Invalid opcode: " + s);
}

bool opcodetab::opcode_exists(string s) {
	s = to_uppercase(s);
	if(opcodemap.find(s) == opcodemap.end())
		return false;
	return true;
}

void opcodetab::add_opcode(string opcode, string machinecode, int size) {
	pair <string, int> value;
	value.first = machinecode;
	value.second = size;
	opcodemap[opcode] = value;
}

string opcodetab::to_uppercase(string s) {
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}
