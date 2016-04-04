// Library v0.2

#ifndef LIBRARY_H
#define LIBRARY_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

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
		std::vector<std::string> patronErrorList;
		std::vector<Patron*> patrons;
		std::vector<Book*> books;

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