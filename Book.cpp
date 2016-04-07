//The Book class v0.7

#include "Book.h"

#ifndef BOOK_CPP
#define BOOK_CPP

Book::Book() {}

Book::~Book() {}

bool Book::checkOut(void* data) {
	if (_checkedOutStatus == false) {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);

		std::strftime(buffer, 80, "%m/%d/%Y", timeinfo);
		_dateCheckedOut = buffer;
		_checkedOutStatus = true;
		_patronData = data;
		//If this returns true, then the book can be successfully checked out.
		return true;
	}
	else {
		//If this returns false, then the book is already checked out and cannot be checked out.
		return false;
	}
}

bool Book::checkIn() {
	if (_checkedOutStatus == true) {
		_dateCheckedOut = "";
		_checkedOutStatus = false;
		_patronData = nullptr;
		//If it returns true, it successfully checked in the book
		return true;
	}
	else {
		//If this returns false, you can't check in an already checked in book.
		return false;
	}
}

void Book::setBookInfo(std::string readlineFromDatabase) {
	std::string buildString;
	int buildStringInt = 0;

	for (int read = 0; read < readlineFromDatabase.length(); read++) {
		if (readlineFromDatabase[read] == ',') {
			if (buildStringInt == 0) {
				_title = buildString;
			}
			else if (buildStringInt == 1) {
				_author = buildString;
			}
			else if (buildStringInt == 2) {
				std::transform(buildString.begin(), buildString.end(), buildString.begin(), ::tolower);
				if (buildString == "adult") {
					_type = 1;
				}
				else if (buildString == "child") {
					_type = 2;
				}
				else if (buildString == "video") {
					_type = 3;
				}
				else if (buildString == "audio") {
					_type = 4;
				}
				else {
					_type = 0;
				}
			}
			buildStringInt++;
			buildString = "";
		}
		else {
			buildString = buildString + readlineFromDatabase[read];
		}
	}
	if (buildStringInt == 3) {
		_ISBN = buildString;
	}
}

const std::string Book::GetTitle() {
	return _title;
}

const std::string Book::GetAuthor() {
	return _author;
}

const int Book::GetType() {
	return _type;
}

const bool Book::GetCheckOutStatus() {
	return _checkedOutStatus;
}

const std::string Book::GetCheckOutDate() {
	return _dateCheckedOut;
}

const std::string Book::GetISBN() {
	return _ISBN;
}

const void* Book::GetPatronData() {
	return _patronData;
}

void Book::Write(std::ostream& out) {
	out << _title << "," << _author << ",";
	if (_type == 1) {
		out << "adult";
	}
	else if (_type == 2) {
		out << "child";
	}
	else if (_type == 3) {
		out << "video";
	}
	else if (_type == 4) {
		out << "audio";
	}
	else {
		out << "unknown";
	}
	out << "," << _ISBN << std::endl;
}



#endif