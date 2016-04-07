//The Book class v0.7

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
	void Write(std::ostream&);
};

#endif