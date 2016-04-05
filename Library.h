// Library v0.3

#ifndef LIBRARY_H
#define LIBRARY_H


#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Date.h"
#include "Book.h"
#include "Patron.h"
#include "Boolinq.h"

typedef std::pair<Patron*, std::vector<Book*>> patronStatus;


class Library 
{
	private:
		const int TYPE_CHILD = 2;
		const size_t MAX_ADULT_CHECKOUT = 5;
		const size_t MAX_CHILD_CHECKOUT = 2;

		std::unordered_map<Patron*, std::vector<Book*>> patronBooks;
		std::vector<std::string> checkoutErrorList;
		std::vector<std::string> patronErrorList;
		std::vector<std::string> bookErrorList;
		std::vector<Patron*> patrons;
		std::vector<Book*> books;

		std::vector<std::string> Split( char*, char );

	public:
		Library();
		~Library();
		void setCurrentDate( std::string );
		void createBooks(std::istream&);
		void createPatrons(std::istream&);
		void restorePBStatus(std::istream&);
		void checkout( std::string, std::string );
		void tryCheckout();
		void writeBooksToDB(std::ostream&);
		void writePatronsToDB(std::ostream&);
		void writePBStatusToDB(std::ostream&);
		bool errorsFound();
		void displayErrors( std::ostream& );
};



#endif