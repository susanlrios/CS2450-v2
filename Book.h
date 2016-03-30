#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include "Patron.h"

class Book {
private:
	std::string _title;
	std::string _author;
	int _type;
	bool _checkedOutStatus = false;
	Patron* _patronCheckOut;
	std::string _dateCheckedOut;
	int _ISBN;
public:
	Book();
	Book(std::string, std::string, std::string, int);
	~Book();
	void checkOut(Patron*);
	void checkIn();
	void printInfo();
};

Book::Book() {}


Book::Book(std::string bookTitle, std::string bookAuthor, std::string bookType, int bookISBNNumber) {
	_title = bookTitle;
	_author = bookAuthor;
	std::transform(bookType.begin(), bookType.end(), bookType.begin(), ::tolower);
	if (bookType == "adult") {
		_type = 1;
	}
	else if (bookType == "child") {
		_type = 2;
	}
	else if (bookType == "video") {
		_type = 3;
	}
	else if (bookType == "audio") {
		_type = 4;
	}
	else {
		_type = 0;
	}
	_ISBN = bookISBNNumber;
}

Book::~Book() {}

void Book::checkOut(Patron* patronName) {
	if (_checkedOutStatus == false) {
		_patronCheckOut = patronName;
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);

		std::strftime(buffer, 80, "%m/%d/%Y", timeinfo);
		_dateCheckedOut = buffer;
		_checkedOutStatus = true;
	}
	else {
		std::cout << "Cannot check out item, item is already checked out by: ";
		std::cout << _patronCheckOut->GetFirstName() << ' ' << _patronCheckOut->GetLastName();
		std::cout << "With ID: " << _patronCheckOut->GetID() << std::endl;
	}
}

void Book::checkIn() {
	if (_checkedOutStatus == true) {
		_patronCheckOut = nullptr;
		_dateCheckedOut = "";
		_checkedOutStatus = false;
	}
	else {
		std::cout << "Cannot check in, already checked in." << std::endl;
	}
}

void Book::printInfo() {
	if (_checkedOutStatus == false) {
		std::cout << "Title: " << _title << ' ' << "Author: " << _author << ' ';
		std::cout << "Date media checked out: " << _dateCheckedOut << ' ';
		std::cout << "ISBN #: " << _ISBN << ' ' << "Person who has checked it out: " << "Nobody" << ' ' << std::endl;
	}
	else if(_checkedOutStatus = true) {
		std::cout << "Title: " << _title << ' ' << "Author: " << _author << ' ';
		std::cout << "Date media checked out: " << _dateCheckedOut << ' ';
		std::cout << "ISBN #: " << _ISBN << ' ' << "Patron who has checked it out: " << _patronCheckOut->GetFirstName();
		std::cout << ' ' << _patronCheckOut->GetLastName() << "With ID: " << _patronCheckOut->GetID() << std::endl;
	}
}
#endif