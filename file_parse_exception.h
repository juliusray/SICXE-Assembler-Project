/*  file_parse_exception.h
    Exception class for file_parser
    CS530 Spring 2014
    Alan Riggins
*/

/*	Travis Barre, Julius Inocencio, Ehsan Eshragh, Michael Reese
	Team Oregon
	masc1187
	prog4
	CS530, Spring 2014
*/

#ifndef FILE_PARSE_EXCEPTION_H
#define FILE_PARSE_EXCEPTION_H
#include <string>

using namespace std;

class file_parse_exception {

public:
	file_parse_exception(string s) {
		message = s;        
	}
        
	file_parse_exception() {
		message = "An error has occurred";
	}
        
	string getMessage() {
		return message;
	}
    
private:
	string message;
};

#endif
