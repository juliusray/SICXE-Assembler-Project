/*	Travis Barre, Ehsan Eshragh, Julius Inocencio, Michael Reese
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include "symtab_exception.h"
#include <string>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

class symtab {
	public:
		// ctor
		// creates an empty dictionary structure to be used by the assembler
		// in order to save user-defined labels for later use
		symtab(); 
		
		string get_address(string);
		bool symbol_exists(string);
		void add_symbol(int, string, string, string);
		void edit_symbol(string);
	private:
		map <string, string > symbol_map;
		string to_uppercase(string);
		string int_to_hex(int, int);
		int str_to_int(string);
		int hex_to_int(string);
		string int_to_str(int);
		void check_numeric(string);
		void check_hex(string);
};

#endif
