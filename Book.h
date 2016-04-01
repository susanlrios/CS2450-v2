//The Book class v0.2

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <ctime>

class Book {
private:
	std::string _title;
	std::string _author;
	int _type;
	bool _checkedOutStatus = false;
	std::string _dateCheckedOut;
	int _ISBN;
public:
	Book();
	~Book();
	bool checkOut();
	bool checkIn();
	void setBookInfo(std::string);
	const std::string GetTitle();
	const std::string GetAuthor();
	const int GetType();
	const bool GetCheckOutStatus();
	const std::string GetCheckOutDate();
	const int GetISBN();
};

#endif