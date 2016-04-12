//The Book class v1.1

#ifndef BOOK_H
#define BOOK_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>

#include "Date.h"

class Book {
private:
	std::string _title;
	std::string _author;
	int _type;
	bool _checkedOutStatus = false;
	std::string _dateCheckedOut;
	std::string _ISBN;
	std::string _patronData;
public:
	Book();
	~Book();
	bool checkOut( std::string );
	bool checkIn();
	void setBookInfo( std::string );
	const std::string GetTitle();
	const std::string GetAuthor();
	const std::string GetType();
	const bool GetCheckOutStatus();
	const std::string GetCheckOutDate();
	const std::string GetISBN();
	const std::string GetPatronID();
	void Write( std::ostream& );
	bool Overdue();
	void Display( std::ostream& );
	void DisplayReduced( std::ostream& );
};

#endif