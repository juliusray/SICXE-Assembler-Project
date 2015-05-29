/*	opcode_error_exception.h
    Exception class for opcodetab
    CS530 Spring 2014
    Alan Riggins
*/

/*	Julius Inocencio, Ehsan Eshragh, Travis Barre, Michael Reese
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H
#include <string>

using namespace std;

class opcode_error_exception {

public:
	opcode_error_exception(string s) {
		message = s;        
	}
        
	opcode_error_exception() {
		message = "An error has occurred";
	}
        
	string getMessage() {
		return message;
	}
    
private:
	string message;
};

#endif
