/*	Travis Barre, Ehsan Eshragh, Julius Inocencio, Michael Reese
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#include "symtab.h"

using namespace std;

// ctor
// creates an empty dictionary structure to be used by the assembler
// in order to save user-defined labels for later use
symtab::symtab() {}

// takes a user-defined label and returns the address
// which is associated with the label
string symtab::get_address(string s){
	if(symbol_exists(s))
		return symbol_map[to_uppercase(s)];
	else
		throw symtab_exception("- Symbol " + s + " has not been defined");
}

bool symtab::symbol_exists(string s){
	return !(symbol_map.find(to_uppercase(s)) == symbol_map.end());
}

void symtab::add_symbol(int x, string symbol, string opcode, string operand){	
	string address;
	if (to_uppercase(opcode) == "EQU") {
		if (operand[0] == '$') {
			check_hex(operand.erase(0,1));
			address = int_to_str(hex_to_int(operand));
		}
		else if(isdigit(operand[0])) {
			check_numeric(operand);
			address = operand;
		}
		else
			try {
				address = get_address(operand);
			} catch(symtab_exception st_excpt) {
				address = operand;
			}
	}
	else if(symbol_exists(symbol))
		throw symtab_exception("- Cannot define " + symbol + " more than once");
	else
		address = x;
		
	symbol_map[to_uppercase(symbol)] = address;
}

void symtab::edit_symbol(string label) {
	if(symbol_exists(get_address(label)))
		symbol_map[label] = (get_address(label));
}

string symtab::to_uppercase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string symtab::int_to_hex(int num, int width){
	stringstream out;
	out << setw(width) << setfill('0') << hex << num;
	return to_uppercase(out.str());
}

int symtab::str_to_int(string s) {
    stringstream stream;
    int value;
    stream << s;
    stream >> value;
    return value;
}

int symtab::hex_to_int(string s){
	int value;
	sscanf(s.c_str(),"%x",&value);
	return value;
}

string symtab::int_to_str(int value){
	stringstream stream;
	string s;
	stream << value;
	stream >> s;
	return s;
}

void symtab::check_numeric(string s) {
	if (s.find_first_not_of("0123456789") != string::npos) {
		throw symtab_exception("Invalid operand" + s);
	}
}

void symtab::check_hex(string s) {
	if (s.find_first_not_of("0123456789ABCDEF") != string::npos) {
		throw symtab_exception("Invalid operand" + s);
	}
}
