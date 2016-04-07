//The Book class v0.5

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
	std::string _ISBN;
	void* _patronData;
public:
	Book();
	~Book();
	bool checkOut(void*);
	bool checkIn();
	void setBookInfo(std::string);
	const std::string GetTitle();
	const std::string GetAuthor();
	const int GetType();
	const bool GetCheckOutStatus();
	const std::string GetCheckOutDate();
	const std::string GetISBN();
	const void* GetPatronData();
};

#endif