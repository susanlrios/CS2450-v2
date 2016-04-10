#ifndef DATE_CPP
#define DATE_CPP

#include "Date.h"


const std::string Date::DATE_PATTERN = R"(^([1][0-2]|[0]?[1-9])/([1-2][0-9]|3[0-2]|[0]?[1-9])/((?:19|20)[0-9]{2})$)";
std::string Date::_currentDate = "";


const std::string Date::CurrentDateTime()
{
	time_t now = time( 0 );
	struct tm tstruct;
	char buffer[80];
	tstruct = *localtime( &now );
	strftime( buffer, sizeof( buffer ), "%m/%d/%Y", &tstruct );

	return buffer;
}


std::vector<std::string> Date::Split( char* input, char delim = ' ' )
{
	std::vector<std::string> splitResult;

	do {
		char *begin = input;

		while ( *input != delim && *input )
		{
			input++;
		}

		splitResult.push_back( std::string( begin, input ) );

	} while ( *input++ != 0 );

	return splitResult;
}


void Date::AddDayToCurrent()
{
	std::vector<std::string> splitVector;
	size_t numberOfDays;
	bool isLeapYear;
	size_t day;
	size_t month;
	size_t year;

	if ( _currentDate == "" ) { _currentDate = GetCurrentDate(); }

	splitVector = Split( &_currentDate[0], '/' );

	month = stoi( splitVector[0] );
	day = stoi( splitVector[1] );
	year = stoi( splitVector[2] );

	day++;

	if ( month == 4 || month == 6 || month == 9 || month == 11 )
	{
		numberOfDays = 30;
	}
	else if ( month == 2 )
	{
		isLeapYear = ( year % 4 == 0 && year % 100 != 0 ) || ( year % 400 == 0 );

		if ( isLeapYear )
		{
			numberOfDays = 29;
		}
		else
		{
			numberOfDays = 28;
		}
	}
	else
	{
		numberOfDays = 31;
	}

	if ( day > numberOfDays )
	{
		day = 1;
		month++;
	}

	if ( month > 12 )
	{
		month = 1;
		day = 1;
		year++;
	}

	_currentDate = std::to_string( month ) + "/" + std::to_string( day ) + "/" + std::to_string( year );
}


void Date::SetCurrentDate( std::string currentDate )
{
	std::vector<std::string> splitVector;
	size_t day;
	size_t month;
	size_t year;

	if ( currentDate == "" ) 
	{ 
		_currentDate = CurrentDateTime();  
	}
	else
	{
		if ( std::regex_match( currentDate, std::regex( DATE_PATTERN ) ) )
		{
			_currentDate = currentDate;
		}
		else
		{
			throw std::logic_error( "Bad argument for current date" );
		}
	}

	splitVector = Split( &_currentDate[0], '/' );

	month = stoi( splitVector[0] );
	day = stoi( splitVector[1] );
	year = stoi( splitVector[2] );

	_currentDate = std::to_string( month ) + "/" + std::to_string( day ) + "/" + std::to_string( year );
}


const std::string& Date::GetCurrentDate()
{
	if ( _currentDate == "" )
	{
		SetCurrentDate( "" );
		return _currentDate;
	}
	else
	{
		return _currentDate;
	}
}


//void Date::operator++( Date& )
//{
//	AddDayToCurrent();
//}



#endif DATE_CPP