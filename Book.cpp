#include "Book.h"

#ifndef BOOK_CPP
#define BOOK_CPP

Book::Book() {}

Book::~Book() {}

bool Book::checkOut( std::string data ) {
	if ( _checkedOutStatus == false ) {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time( &rawtime );
		timeinfo = std::localtime( &rawtime );

		std::strftime( buffer, 80, "%m/%d/%Y", timeinfo );
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
	if ( _checkedOutStatus == true ) {
		_dateCheckedOut = "";
		_checkedOutStatus = false;
		_patronData = "";
		//If it returns true, it successfully checked in the book
		return true;
	}
	else {
		//If this returns false, you can't check in an already checked in book.
		return false;
	}
}

void Book::setBookInfo( std::string readlineFromDatabase ) {
	std::string buildString;
	int buildStringInt = 0;

	for ( int read = 0; read < readlineFromDatabase.length(); read++ ) {
		if ( readlineFromDatabase[read] == ',' ) {
			if ( buildStringInt == 0 ) {
				_title = buildString;
			}
			else if ( buildStringInt == 1 ) {
				_author = buildString;
			}
			else if ( buildStringInt == 2 ) {
				std::transform( buildString.begin(), buildString.end(), buildString.begin(), ::tolower );
				if ( buildString == "adult" ) {
					_type = 1;
				}
				else if ( buildString == "child" ) {
					_type = 2;
				}
				else if ( buildString == "video" ) {
					_type = 3;
				}
				else if ( buildString == "dvd" ) {
					_type = 4;
				}
				else {
					throw std::logic_error( "Book class failed to create successfully." );
				}
			}
			else if ( buildStringInt == 3 ) {
				_ISBN = buildString;
			}
			else if ( buildStringInt == 4 ) {
				_dateCheckedOut = buildString;
			}
			buildStringInt++;
			buildString = "";
		}
		else {
			buildString = buildString + readlineFromDatabase[read];
		}
	}
	if ( buildStringInt == 3 ) {
		_ISBN = buildString;
	}
	if ( buildStringInt == 4 ) {
		_dateCheckedOut = buildString;
	}
	else if ( buildStringInt > 5 ) {
		throw std::logic_error( "Book class failed to create successfully." );
	}
}

const std::string Book::GetTitle() {
	return _title;
}

const std::string Book::GetAuthor() {
	return _author;
}

const std::string Book::GetType() {
	switch ( _type )
	{
	case 1:
		return "adult";
		break;
	case 2:
		return "child";
		break;
	case 3:
		return "video";
		break;
	case 4:
		return "dvd";
		break;
	default:
		return "-1";
		break;
	}
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

const std::string Book::GetPatronID() {
	return _patronData;
}

void Book::Write( std::ostream& out ) {
	out << _title << "," << _author << ",";
	if ( _type == 1 ) {
		out << "adult";
	}
	else if ( _type == 2 ) {
		out << "child";
	}
	else if ( _type == 3 ) {
		out << "video";
	}
	else if ( _type == 4 ) {
		out << "dvd";
	}
	out << "," << _ISBN << "," << _dateCheckedOut << (( _dateCheckedOut != "" ) ? "," : "") << std::endl;
}

bool Book::Overdue() {
	int days = 0;
	if ( _dateCheckedOut == "" ) {
		return false;
	}
	std::string _storeCurrentDate;
	_storeCurrentDate = Date::GetCurrentDate();
	Date::SetCurrentDate( _dateCheckedOut );
	while ( Date::GetCurrentDate() != _storeCurrentDate ) {
		Date::AddDayToCurrent();
		days++;
	}
	Date::SetCurrentDate( _storeCurrentDate );
	if ( _type == 1 ) {
		if ( days >= 14 ) {
			return true;
		}
		else {
			return false;
		}
	}
	else if ( _type == 2 ) {
		if ( days >= 7 ) {
			return true;
		}
		else {
			return false;
		}
	}
	else if ( _type == 3 ) {
		if ( days >= 2 ) {
			return true;
		}
		else {
			return false;
		}
	}
	else if ( _type == 4 ) {
		if ( days >= 3 ) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Book::Display( std::ostream& out ) {
	out << _title << std::endl;
	out << "\t" << "Author: " << _author << std::endl;
	if ( _type == 1 ) {
		out << "\t" << "Type: " << "adult" << std::endl;
	}
	else if ( _type == 2 ) {
		out << "\t" << "Type: " << "child" << std::endl;
	}
	else if ( _type == 3 ) {
		out << "\t" << "Type: " << "video" << std::endl;
	}
	else if ( _type == 4 ) {
		out << "\t" << "Type: " << "dvd" << std::endl;
	}
	out << "\t" << "ISBN: " << _ISBN << std::endl;
	out << "\t" << "Status: " << ( ( _checkedOutStatus ) ? "Checked-out" : "Available" ) << std::endl;
}

void Book::DisplayReduced( std::ostream& out )
{
	out << "ISBN: " << _ISBN << "\t" << _title << std::endl <<
		"\tStatus: " << ( ( _checkedOutStatus ) ? "Checked-out" : "Available" ) << std::endl;
}

#endif