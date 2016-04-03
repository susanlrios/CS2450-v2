#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "Book.h"
#include "Patron.h"

class Library {
private:
	std::unordered_map<Patron*, std::vector<Book*>> patronBooks;
	std::vector<Patron*> patrons;
	std::vector<Book*> books;
	std::ostringstream convert;
	std::string input;
public:
	Library();
	~Library();
	void createBooks(std::istream&);
	void createPatrons(std::istream&);
	void restorePBStatus(std::istream&);
	void writeBooksToDB(std::ostream&);
	void writePatronsToDB(std::ostream&);
	void writePBStatusToDB(std::ostream&);
	void tryCheckout();
};

#endif